#include "types.h"
#include "vectors.inc"

#define DISPATCH_DIRECT(prefix)                                               \
  switch (shape) {                                                            \
  case 0:                                                                     \
    return prefix##0();                                                       \
  case 1:                                                                     \
    return prefix##1(a0);                                                     \
  case 2:                                                                     \
    return prefix##2(a0, a1);                                                 \
  case 3:                                                                     \
    return prefix##3(a0, a1, a2);                                             \
  case 4:                                                                     \
    return prefix##4(a0, a1, a2, a3);                                         \
  case 5:                                                                     \
    return prefix##5(a0, a1, a2, a3, a4);                                     \
  case 6:                                                                     \
    return prefix##6(a0, a1, a2, a3, a4, a5);                                 \
  case 7:                                                                     \
    return prefix##7(a0, a1, a2, a3, a4, a5, a6);                             \
  case 8:                                                                     \
    return prefix##8(a0, a1, a2, a3, a4, a5, a6, a7);                         \
  case 9:                                                                     \
    return prefix##9(a0, a1, a2, a3, a4, a5, a6, a7, a8);                     \
  default:                                                                    \
    return prefix##10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);               \
  }

static abi_u32 call_direct(abi_u16 shape, abi_u16 a0, abi_u16 a1, abi_u16 a2,
                           abi_u16 a3, abi_u16 a4, abi_u16 a5, abi_u16 a6,
                           abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  DISPATCH_DIRECT(words)
}

#undef DISPATCH_DIRECT

static abi_u32 call_indirect(abi_u16 shape, abi_u16 a0, abi_u16 a1,
                             abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
                             abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  switch (shape) {
  case 0: {
    volatile word_fn0 function = words0;
    return function();
  }
  case 1: {
    volatile word_fn1 function = words1;
    return function(a0);
  }
  case 2: {
    volatile word_fn2 function = words2;
    return function(a0, a1);
  }
  case 3: {
    volatile word_fn3 function = words3;
    return function(a0, a1, a2);
  }
  case 4: {
    volatile word_fn4 function = words4;
    return function(a0, a1, a2, a3);
  }
  case 5: {
    volatile word_fn5 function = words5;
    return function(a0, a1, a2, a3, a4);
  }
  case 6: {
    volatile word_fn6 function = words6;
    return function(a0, a1, a2, a3, a4, a5);
  }
  case 7: {
    volatile word_fn7 function = words7;
    return function(a0, a1, a2, a3, a4, a5, a6);
  }
  case 8: {
    volatile word_fn8 function = words8;
    return function(a0, a1, a2, a3, a4, a5, a6, a7);
  }
  case 9: {
    volatile word_fn9 function = words9;
    return function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
  }
  default: {
    volatile word_fn10 function = words10;
    return function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
  }
}

static void run_vector(
    abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
    abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9, abi_u32 golden0,
    abi_u32 golden1, abi_u32 golden2, abi_u32 golden3, abi_u32 golden4,
    abi_u32 golden5, abi_u32 golden6, abi_u32 golden7, abi_u32 golden8,
    abi_u32 golden9, abi_u32 golden10) {
  abi_u32 goldens[11];
  abi_u16 shape;

  goldens[0] = golden0;
  goldens[1] = golden1;
  goldens[2] = golden2;
  goldens[3] = golden3;
  goldens[4] = golden4;
  goldens[5] = golden5;
  goldens[6] = golden6;
  goldens[7] = golden7;
  goldens[8] = golden8;
  goldens[9] = golden9;
  goldens[10] = golden10;

  for (shape = 0; shape != 11U; ++shape) {
    tap_is_u32(call_direct(shape, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9),
               goldens[shape], "direct word arguments");
    tap_is_u32(reverse_words(shape, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9),
               goldens[shape], "nested word arguments");
    tap_is_u32(call_indirect(shape, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9),
               goldens[shape], "indirect word arguments");
  }
}

#define RUN_VECTOR(id, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, g0, g1,    \
                   g2, g3, g4, g5, g6, g7, g8, g9, g10)                     \
  run_vector(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, g0, g1, g2, g3, g4,  \
             g5, g6, g7, g8, g9, g10);

void main(void) {
  tap_plan(33U);
  ABI_WORD_MATRIX_VECTORS(RUN_VECTOR)
}
