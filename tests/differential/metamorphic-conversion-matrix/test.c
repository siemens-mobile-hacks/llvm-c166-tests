#include "c166_test.h"
#include "types.h"

#include "vectors.inc"

extern abi_u32 c166_metamorphic_eval(abi_u16, abi_u32, abi_s32, abi_u32,
                                     abi_u16, abi_s8, abi_u8, abi_u16);

static void run_vector(abi_u16 operation, abi_u32 x, abi_s32 sx, abi_u32 y,
                       abi_u16 aux, abi_s8 sc, abi_u8 uc, abi_u16 tail,
                       abi_u32 expected) {
  tap_is_u32(c166_metamorphic_eval(operation, x, sx, y, aux, sc, uc, tail),
             expected, "metamorphic conversion");
}

#define RUN_VECTOR(id, operation, x, sx, y, aux, sc, uc, tail, expected)       \
  run_vector(operation, x, sx, y, aux, sc, uc, tail, expected);

void main(void) {
  tap_plan(C166_METAMORPHIC_VECTOR_COUNT);
  C166_METAMORPHIC_VECTORS(RUN_VECTOR)
}
