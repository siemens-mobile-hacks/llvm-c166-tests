#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u32 llvm_reverse_words_proxy(
    abi_u16 shape, abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3,
    abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9);

#define DISPATCH(prefix) \
  switch (shape) { \
  case 0: return prefix##0(); \
  case 1: return prefix##1(a0); \
  case 2: return prefix##2(a0, a1); \
  case 3: return prefix##3(a0, a1, a2); \
  case 4: return prefix##4(a0, a1, a2, a3); \
  case 5: return prefix##5(a0, a1, a2, a3, a4); \
  case 6: return prefix##6(a0, a1, a2, a3, a4, a5); \
  case 7: return prefix##7(a0, a1, a2, a3, a4, a5, a6); \
  case 8: return prefix##8(a0, a1, a2, a3, a4, a5, a6, a7); \
  case 9: return prefix##9(a0, a1, a2, a3, a4, a5, a6, a7, a8); \
  default: return prefix##10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9); \
  }

static abi_u32 call_tasking(abi_u16 shape, abi_u16 a0, abi_u16 a1,
                            abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
                            abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  DISPATCH(tasking_words)
}

static abi_u32 call_llvm(abi_u16 shape, abi_u16 a0, abi_u16 a1, abi_u16 a2,
                         abi_u16 a3, abi_u16 a4, abi_u16 a5, abi_u16 a6,
                         abi_u16 a7, abi_u16 a8, abi_u16 a9) {
  DISPATCH(llvm_words_proxy)
}

#undef DISPATCH

static void run_word_matrix_vector(
    abi_u16 vector_id, abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3,
    abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9,
    abi_u32 golden0, abi_u32 golden1, abi_u32 golden2, abi_u32 golden3,
    abi_u32 golden4, abi_u32 golden5, abi_u32 golden6, abi_u32 golden7,
    abi_u32 golden8, abi_u32 golden9, abi_u32 golden10) {
  abi_u32 goldens[11];
  abi_u32 actual;
  abi_u16 shape;
  abi_u16 base;

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

#define CHECK_ROUTE(case_id, golden, expression) \
  do { \
    actual = (expression); \
    c166_test_check_u32((case_id), (golden), actual); \
  } while (0)

  for (shape = 0; shape <= 10; ++shape) {
    base = (abi_u16)((vector_id - 1) * 33 + shape * 3);
    CHECK_ROUTE(base + 1, goldens[shape],
                call_tasking(shape, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9));
    CHECK_ROUTE(base + 2, goldens[shape],
                call_llvm(shape, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9));
    CHECK_ROUTE(base + 3, goldens[shape],
                llvm_reverse_words_proxy(shape, a0, a1, a2, a3, a4, a5,
                                         a6, a7, a8, a9));
  }

#undef CHECK_ROUTE
}

#define RUN_WORD_MATRIX(id, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, \
                        g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10) \
  run_word_matrix_vector(id, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, \
                         g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10);

void main(void) {
  c166_test_begin(269, 0x1660010dUL);
  ABI_WORD_MATRIX_VECTORS(RUN_WORD_MATRIX)
  c166_test_finish();
  simulator_stop();
}

