#include "c166_test.h"
#include "vectors.inc"

typedef unsigned int test_u16;
typedef unsigned long test_u32;

test_u32 C166_HUGE copy_pair_up_probe(test_u16 low, test_u16 high);
test_u32 C166_HUGE copy_pair_down_probe(test_u16 low, test_u16 high);
test_u32 C166_HUGE zext_pair_high_overlap_probe(test_u16 value);
test_u32 C166_HUGE sext_pair_high_overlap_probe(test_u16 value);

static void run_pair(test_u16 low, test_u16 high, test_u32 expected) {
  tap_is_u32(copy_pair_up_probe(low, high), expected, "copy pair up");
  tap_is_u32(copy_pair_down_probe(low, high), expected, "copy pair down");
}

static void run_scalar(test_u16 value, test_u32 zero_extended,
                       test_u32 sign_extended) {
  tap_is_u32(zext_pair_high_overlap_probe(value), zero_extended,
             "zero extend overlap");
  tap_is_u32(sext_pair_high_overlap_probe(value), sign_extended,
             "sign extend overlap");
}

#define RUN_PAIR(id, low, high, expected) run_pair(low, high, expected);
#define RUN_SCALAR(id, value, zero_extended, sign_extended) \
  run_scalar(value, zero_extended, sign_extended);

void main(void) {
  tap_plan(ABI_TUPLE_PAIR_CASES + 10U);
  ABI_TUPLE_PAIR_VECTORS(RUN_PAIR)
  ABI_TUPLE_SCALAR_VECTORS(RUN_SCALAR)
}
