#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int operation, unsigned int lhs,
                                     unsigned int rhs, unsigned int flags);

static const unsigned int words[] = {
    0, 1, 2, 0x7fff, 0x8000, 0xfffe, 0xffff, 0xa55a};
static const unsigned int bytes[] = {0, 1, 2, 0x7f, 0x80, 0xfe, 0xff, 0x5a};
static const unsigned int immediates[] = {8, 16, 127, 128, 254, 255, 90, 64};
static const unsigned int sfr_immediates[] = {
    0, 1, 7, 0x7fff, 0x8000, 0xfffe, 0xffff, 0xa55a};
static const unsigned int loop_short[] = {0, 1, 2, 7, 8, 9, 14, 15};
static const unsigned int loop_long[] = {
    16, 17, 255, 256, 0x7fff, 0x8000, 0xfffe, 0xffff};

/* ISA rules, evaluated at 32 bits to avoid 16-bit arithmetic overflow. */
static unsigned long expected(unsigned int op, unsigned int a, unsigned int b,
                              unsigned int initial) {
  unsigned int result, flags, carry = 0, overflow = 0;
  unsigned int chained, input_carry;
  unsigned int byte = (op >= 15 && op < 40) || (op >= 71 && op < 114);
  unsigned int updated = a;
  unsigned int mask = byte ? 255U : 65535U;
  unsigned int sign = byte ? 128U : 32768U;
  long sa, sb;
  long signed_result;
  unsigned long wide;
  a &= mask;
  b &= mask;
  sa = a < sign ? (long)a : (long)a - ((long)mask + 1);
  sb = b < sign ? (long)b : (long)b - ((long)mask + 1);
  if (op >= 126) op = (op - 126) % 8;
  else if (op >= 114) {
    static const int changes[] = {-1, -2, 1, 2};
    updated = a + changes[(op - 114) % 4];
    op = 4; /* Flags describe the comparison before the register update. */
  }
  else if (op >= 106) op = op - 106 + 15;
  else if (op >= 99) op = op - 99 + 23;
  else if (op >= 91) op = op - 91 + 15;
  else if (op >= 83) op = op - 83 + 15;
  else if (op >= 71) {
    static const unsigned char operations[] = {15, 17, 19, 20, 21, 22};
    op = operations[(op - 71) % 6];
  } else if (op >= 63) op -= 63;
  else if (op >= 48) op -= 48;
  else if (op >= 40) op -= 40;
  else if (op >= 30) op = (op & 1U) ? 18 : 16;
  if (byte) op -= 15;
  if (op >= 8) {
    op -= 8;
    if (op >= 4) ++op; /* CMP has no memory-destination form. */
  }
  chained = op == 1 || op == 3;
  input_carry = chained ? ((initial >> 1) & 1U) : 0;
  if (op < 2) {
    wide = (unsigned long)a + b + input_carry;
    result = (unsigned int)wide;
    carry = wide > mask;
    signed_result = sa + sb + input_carry;
    overflow = signed_result < -(long)sign || signed_result >= (long)sign;
  } else if (op < 5) {
    wide = (unsigned long)b + input_carry;
    result = (unsigned int)((unsigned long)a - wide);
    carry = (unsigned long)a < wide;
    signed_result = sa - sb - input_carry;
    overflow = signed_result < -(long)sign || signed_result >= (long)sign;
  } else if (op == 5) {
    result = a ^ b;
  } else if (op == 6) {
    result = a & b;
  } else {
    result = a | b;
  }
  result &= mask;
  flags = ((result & sign) != 0) | (carry << 1) | (overflow << 2);
  if (result == 0 && (!chained || (initial & 8U))) flags |= 8U;
  if (b == sign) flags |= 16U;
  return ((unsigned long)flags << 16) | (op == 4 ? (updated & mask) : result) |
         (byte ? 0xa500U : 0);
}

void main(void) {
  unsigned int op, lhs, rhs, flags, a, b, id = 0;
  unsigned long want, actual;
  c166_test_begin(370, 0x166UL);
  for (op = 0; op != 142; ++op) {
    id = 0;
    for (lhs = 0; lhs != 8; ++lhs)
      for (rhs = 0; rhs != 8; ++rhs)
        for (flags = 0; flags != 32; ++flags) {
          a = (op < 15 || (op >= 40 && op < 71) || op >= 114)
                  ? words[lhs] : (0xa500U | bytes[lhs]);
          if (op >= 122)
            b = words[rhs];
          else if (op >= 118)
            b = loop_long[rhs];
          else if (op >= 114)
            b = loop_short[rhs];
          else if (op >= 71)
            b = 0x5a00U | bytes[rhs];
          else if (op >= 48)
            b = words[rhs];
          else if (op >= 40)
            b = sfr_immediates[rhs];
          else if (op >= 36)
            b = 0x5a00U | (op < 38 ? rhs : immediates[rhs]);
          else
            b = op < 15 ? words[rhs] : (0x5a00U | bytes[rhs]);
          c166_test_set_context(0x166UL, op, id, a, b);
          want = expected(op, a, b, flags);
          actual = llvm_entry_proxy(op, a, b, flags);
          c166_test_check_u32(++id, want, actual);
          if (actual != want) goto done;
        }
  }
done:
  c166_test_finish();
  simulator_stop();
}
