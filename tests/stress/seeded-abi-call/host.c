#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "functions.h"
#include "corpus.inc"

extern abi_u32 llvm_reverse_mixed_proxy(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail);

struct prng_state {
  abi_u16 low;
  abi_u16 high;
};

static abi_u16 tasking_callback(abi_u16 value) {
  return value ^ 0x5aa5U;
}

static abi_u16 xorshift16(abi_u16 value) {
  value ^= (abi_u16)(value << 7);
  value ^= value >> 9;
  value ^= (abi_u16)(value << 8);
  return value;
}

static abi_u32 next_u32(struct prng_state *state) {
  state->low = xorshift16(state->low);
  state->high = xorshift16(state->high);
  if (state->low == 0U && state->high == 0U)
    state->high = 1U;
  return ((abi_u32)state->high << 16) | state->low;
}

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 value) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ value ^ tag;
}

#define ABI_DISPATCH(prefix) \
  switch (shape) { \
  case 0: return prefix##s8_0(signed_byte, tail); \
  case 1: return prefix##s8_1(p0, signed_byte, tail); \
  case 2: return prefix##s8_2(p0, p1, signed_byte, tail); \
  case 3: return prefix##s8_3(p0, p1, p2, signed_byte, tail); \
  case 4: return prefix##s8_4(p0, p1, p2, p3, signed_byte, tail); \
  case 5: return prefix##u8_0(unsigned_byte, tail); \
  case 6: return prefix##u8_1(p0, unsigned_byte, tail); \
  case 7: return prefix##u8_2(p0, p1, unsigned_byte, tail); \
  case 8: return prefix##u8_3(p0, p1, p2, unsigned_byte, tail); \
  case 9: return prefix##u8_4(p0, p1, p2, p3, unsigned_byte, tail); \
  case 10: return prefix##u32_0(long_value, tail); \
  case 11: return prefix##u32_1(p0, long_value, tail); \
  case 12: return prefix##u32_2(p0, p1, long_value, tail); \
  case 13: return prefix##u32_3(p0, p1, p2, long_value, tail); \
  case 14: return prefix##ptr_0(address, tail); \
  case 15: return prefix##ptr_1(p0, address, tail); \
  case 16: return prefix##ptr_2(p0, p1, address, tail); \
  case 17: return prefix##ptr_3(p0, p1, p2, address, tail); \
  case 18: return prefix##fn_0(function, tail); \
  case 19: return prefix##fn_1(p0, function, tail); \
  case 20: return prefix##fn_2(p0, p1, function, tail); \
  default: return prefix##fn_3(p0, p1, p2, function, tail); \
  }

static abi_u32 call_tasking(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  ABI_DISPATCH(tasking_)
}

static abi_u32 call_llvm(
    abi_u16 shape, abi_u16 p0, abi_u16 p1, abi_u16 p2, abi_u16 p3,
    abi_s8 signed_byte, abi_u8 unsigned_byte, abi_u32 long_value,
    volatile abi_u16 *address, abi_callback function, abi_u16 tail) {
  ABI_DISPATCH(llvm_proxy_)
}

#undef ABI_DISPATCH

static void run_seed(abi_u16 seed_index, abi_u32 seed) {
  struct prng_state state;
  abi_u32 signatures[ABI_SEEDED_CALL_SIGNATURE_COUNT]
                    [ABI_SEEDED_CALL_ROUTE_COUNT];
  abi_u32 raw0;
  abi_u32 raw1;
  abi_u32 long_value;
  abi_u32 misc;
  abi_u32 actual;
  abi_u16 p0;
  abi_u16 p1;
  abi_u16 p2;
  abi_u16 p3;
  abi_s8 signed_byte;
  abi_u8 unsigned_byte;
  volatile abi_u16 cell;
  abi_u16 tail;
  abi_u16 iteration;
  abi_u16 shape;
  abi_u16 route;
  abi_u16 checkpoint = 0U;

  state.low = (abi_u16)seed;
  state.high = (abi_u16)(seed >> 16);
  for (shape = 0U; shape < ABI_SEEDED_CALL_SIGNATURE_COUNT; ++shape)
    for (route = 0U; route < ABI_SEEDED_CALL_ROUTE_COUNT; ++route)
      signatures[shape][route] = ABI_SEEDED_CALL_GROUP_SIGNATURE_SEED ^
          ((abi_u32)(seed_index + 1U) << 12) ^
          ((abi_u32)shape << 4) ^ route;

  for (iteration = 0U; iteration < ABI_SEEDED_CALL_ITERATIONS; ++iteration) {
    raw0 = next_u32(&state);
    raw1 = next_u32(&state);
    long_value = next_u32(&state);
    misc = next_u32(&state);
    p0 = (abi_u16)raw0;
    p1 = (abi_u16)(raw0 >> 16);
    p2 = (abi_u16)raw1;
    p3 = (abi_u16)(raw1 >> 16);
    signed_byte = (abi_s8)long_value;
    unsigned_byte = (abi_u8)(long_value >> 8);
    cell = (abi_u16)misc;
    tail = (abi_u16)(misc >> 16);
    if ((iteration & 7U) == 0U) {
      p0 = 0U; p1 = 0xffffU; p2 = 0x8000U; p3 = 0x7fffU;
      signed_byte = (abi_s8)0x80U; unsigned_byte = 0xffU;
      long_value = 0UL; cell = 0U; tail = 0U;
    } else if ((iteration & 7U) == 1U) {
      p0 = 0xffffU; p1 = 0U; p2 = 0x7fffU; p3 = 0x8000U;
      signed_byte = (abi_s8)0x7fU; unsigned_byte = 0U;
      long_value = 0xffffffffUL; cell = 0xffffU; tail = 0xffffU;
    }

    for (shape = 0U; shape < ABI_SEEDED_CALL_SIGNATURE_COUNT; ++shape) {
      for (route = 0U; route < ABI_SEEDED_CALL_ROUTE_COUNT; ++route) {
        abi_u16 expected_index;
        abi_u16 case_id;

        if (route == 0U)
          actual = call_tasking(shape, p0, p1, p2, p3, signed_byte,
              unsigned_byte, long_value, &cell, tasking_callback, tail);
        else if (route == 1U)
          actual = call_llvm(shape, p0, p1, p2, p3, signed_byte,
              unsigned_byte, long_value, &cell, tasking_callback, tail);
        else
          actual = llvm_reverse_mixed_proxy(
              shape, p0, p1, p2, p3, signed_byte, unsigned_byte,
              long_value, &cell, tasking_callback, tail);
        signatures[shape][route] = signature_step(
            signatures[shape][route], (abi_u16)(iteration + 1U), actual);
        if ((iteration & (ABI_SEEDED_CALL_CHECKPOINT_INTERVAL - 1U)) ==
            ABI_SEEDED_CALL_CHECKPOINT_INTERVAL - 1U) {
          expected_index = (abi_u16)(
              seed_index * ABI_SEEDED_CALL_SIGNATURE_COUNT *
                  ABI_SEEDED_CALL_ROUTE_COUNT *
                  ABI_SEEDED_CALL_CHECKPOINTS_PER_ROUTE +
              shape * ABI_SEEDED_CALL_ROUTE_COUNT *
                  ABI_SEEDED_CALL_CHECKPOINTS_PER_ROUTE +
              route * ABI_SEEDED_CALL_CHECKPOINTS_PER_ROUTE + checkpoint);
          case_id = (abi_u16)(
              seed_index * ABI_SEEDED_CALL_SIGNATURE_COUNT *
                  ABI_SEEDED_CALL_ROUTE_COUNT *
                  ABI_SEEDED_CALL_CHECKPOINTS_PER_ROUTE +
              checkpoint * ABI_SEEDED_CALL_SIGNATURE_COUNT *
                  ABI_SEEDED_CALL_ROUTE_COUNT +
              shape * ABI_SEEDED_CALL_ROUTE_COUNT + route + 1U);
          c166_test_set_context(seed,
              (abi_u16)(shape * ABI_SEEDED_CALL_ROUTE_COUNT + route),
              iteration, long_value, ((abi_u32)cell << 16) | tail);
          c166_test_check_u32(case_id,
              abi_seeded_call_expected[expected_index],
              signatures[shape][route]);
        }
      }
    }
    if ((iteration & (ABI_SEEDED_CALL_CHECKPOINT_INTERVAL - 1U)) ==
        ABI_SEEDED_CALL_CHECKPOINT_INTERVAL - 1U)
      ++checkpoint;
  }
}

void main(void) {
  abi_u16 seed_index;

  c166_test_begin(ABI_SEEDED_CALL_SUITE_ID, 0x16600143UL);
  for (seed_index = 0U; seed_index < ABI_SEEDED_CALL_SEED_COUNT; ++seed_index)
    run_seed(seed_index, abi_seeded_call_seeds[seed_index]);
  c166_test_finish();
  simulator_stop();
}

