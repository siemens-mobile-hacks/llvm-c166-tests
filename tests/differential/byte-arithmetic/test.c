#include "c166-test-compat.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

u16 size_byte_compare(u8, u8);
u16 size_byte_compare_immediate(u8);
u16 size_byte_compare_full_immediate(u8);
void size_byte_increment(u8 *);
void size_byte_decrement(u8 *);
void size_byte_xor(u8 *);
void size_byte_and(u8 *);
void size_byte_or(u8 *);
void size_byte_add_full_immediate(u8 *);
void size_byte_subtract_full_immediate(u8 *);
void size_byte_xor_full_immediate(u8 *);
void size_byte_and_full_immediate(u8 *);
void size_byte_or_full_immediate(u8 *);
void size_byte_negate(u8 *);
void size_byte_complement(u8 *);
void size_byte_add_value(u8 *, const u8 *);
void size_byte_subtract_value(u8 *, const u8 *);
void size_byte_xor_value(u8 *, const u8 *);
void size_byte_and_value(u8 *, const u8 *);
void size_byte_or_value(u8 *, const u8 *);

C166_TEST_NOINLINE u32 c166_test_case(u16 case_id) {
  u8 value;
  u8 operand;

  switch (case_id) {
  case 0:
    return size_byte_compare(0x80u, 0x7fu);
  case 1:
    return size_byte_compare_immediate(2u);
  case 2:
    return size_byte_compare_immediate(3u);
  case 3:
    value = 0xffu;
    size_byte_increment(&value);
    return value;
  case 4:
    value = 0u;
    size_byte_decrement(&value);
    return value;
  case 5:
    value = 0xa5u;
    size_byte_xor(&value);
    return value;
  case 6:
    value = 0xffu;
    size_byte_and(&value);
    return value;
  case 7:
    value = 0x80u;
    size_byte_or(&value);
    return value;
  case 8:
    value = 0x80u;
    size_byte_negate(&value);
    return value;
  case 9:
    value = 0x55u;
    size_byte_complement(&value);
    return value;
  case 10:
    value = 0xfeu;
    operand = 5u;
    size_byte_add_value(&value, &operand);
    return value;
  case 11:
    value = 2u;
    operand = 5u;
    size_byte_subtract_value(&value, &operand);
    return value;
  case 12:
    value = 0xa5u;
    operand = 0x3cu;
    size_byte_xor_value(&value, &operand);
    return value;
  case 13:
    value = 0xa5u;
    operand = 0x3cu;
    size_byte_and_value(&value, &operand);
    return value;
  case 14:
    value = 0xa5u;
    operand = 0x3cu;
    size_byte_or_value(&value, &operand);
    return value;
  case 15:
    return size_byte_compare_full_immediate(199u);
  case 16:
    return size_byte_compare_full_immediate(200u);
  case 17:
    value = 100u;
    size_byte_add_full_immediate(&value);
    return value;
  case 18:
    value = 2u;
    size_byte_subtract_full_immediate(&value);
    return value;
  case 19:
    value = 0xa5u;
    size_byte_xor_full_immediate(&value);
    return value;
  case 20:
    value = 0xa5u;
    size_byte_and_full_immediate(&value);
    return value;
  default:
    value = 0x24u;
    size_byte_or_full_immediate(&value);
    return value;
  }
}
