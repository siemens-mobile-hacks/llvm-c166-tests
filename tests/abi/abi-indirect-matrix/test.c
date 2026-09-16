#include "types.h"
#include "vectors.inc"

static void run_indirect_matrix(abi_u16 a, abi_u16 b, abi_u16 c, abi_u16 d,
                                abi_u16 e, abi_u32 golden0, abi_u32 golden2,
                                abi_u32 golden5) {
  abi_fn0 function0;
  abi_fn2 function2;
  abi_fn5 function5;
  abi_fn0 volatile slot0;
  abi_fn2 volatile slot2;
  abi_fn5 volatile slot5;

  tap_is_u32(indirect_target0(), golden0, "direct zero-argument call");
  tap_is_u32(indirect_call0(&slot0, indirect_target0), golden0,
             "stored zero-argument indirect call");
  tap_ok(slot0 == indirect_target0, "zero-argument pointer identity");
  function0 = indirect_get0();
  tap_ok(function0 == indirect_get0(), "zero-argument getter identity");
  slot0 = function0;
  tap_is_u32(slot0(), golden0, "returned zero-argument pointer call");

  tap_is_u32(indirect_target2(a, b), golden2, "direct register call");
  tap_is_u32(indirect_call2(&slot2, indirect_target2, a, b), golden2,
             "stored register indirect call");
  tap_ok(slot2 == indirect_target2, "register-call pointer identity");
  function2 = indirect_get2();
  tap_ok(function2 == indirect_get2(), "register-call getter identity");
  slot2 = function2;
  tap_is_u32(slot2(a, b), golden2, "returned register-call pointer");

  tap_is_u32(indirect_target5(a, b, c, d, e), golden5,
             "direct stack-argument call");
  tap_is_u32(indirect_call5(&slot5, indirect_target5, a, b, c, d, e),
             golden5, "stored stack-argument indirect call");
  tap_ok(slot5 == indirect_target5, "stack-call pointer identity");
  function5 = indirect_get5();
  tap_ok(function5 == indirect_get5(), "stack-call getter identity");
  slot5 = function5;
  tap_is_u32(slot5(a, b, c, d, e), golden5,
             "returned stack-call pointer");

  tap_is_u32(indirect_target0(), golden0,
             "zero-argument call after indirect calls");
  tap_is_u32(indirect_target2(a, b), golden2,
             "register call after indirect calls");
  tap_is_u32(indirect_target5(a, b, c, d, e), golden5,
             "stack-argument call after indirect calls");
}

#define RUN_INDIRECT_MATRIX(a, b, c, d, e, golden0, golden2, golden5)       \
  run_indirect_matrix(a, b, c, d, e, golden0, golden2, golden5);

void main(void) {
  tap_plan(18U);
  ABI_INDIRECT_MATRIX_VECTOR(RUN_INDIRECT_MATRIX)
}
