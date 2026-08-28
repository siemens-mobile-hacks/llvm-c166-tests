#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "matrix-common.h"
#include "vectors.inc"

DECLARE_VARARGS(llvm_varargs_proxy);

extern abi_u32 llvm_reverse_entry_proxy(
    abi_u16 shape, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value, abi_ull long_long_value,
    volatile abi_u16 *pointer_value);
extern abi_u32 llvm_varargs_stream_proxy(abi_u16 prefix_count, ...);
extern abi_u32 llvm_reverse_stream_entry_proxy(
    abi_u16 prefix_count, volatile abi_u16 *pointer_value);

volatile abi_u16 cells[4];
volatile abi_u16 stream_cells[8];

static void run_varargs_matrix_vector(
    abi_u16 vector_id, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value, abi_ull long_long_value,
    abi_u16 pointed_value,
    abi_u32 golden1, abi_u32 golden2, abi_u32 golden3, abi_u32 golden4,
    abi_u32 golden5) {
  volatile abi_u16 *pointer_value = &cells[vector_id - 1];
  abi_u32 reference;
  abi_u32 actual;
  abi_u32 reverse;
  abi_u16 base;

  cells[vector_id - 1] = pointed_value;

#define CHECK_SHAPE(shape, golden, reference_call, actual_call) \
  do { \
    reference = (reference_call); \
    actual = (actual_call); \
    reverse = llvm_reverse_entry_proxy( \
        shape, fixed0, fixed1, fixed2, fixed3, fixed4, signed_value, \
        unsigned_byte, enum_value, word_value, long_value, long_long_value, \
        pointer_value); \
    base = (abi_u16)((vector_id - 1) * 15 + ((shape) - 1) * 3); \
    c166_test_check_u32(base + 1, golden, reference); \
    c166_test_check_u32(base + 2, golden, actual); \
    c166_test_check_u32(base + 3, golden, reverse); \
  } while (0)

  CHECK_SHAPE(
      1, golden1,
      (tasking_varargs1(fixed0, signed_value, unsigned_byte, enum_value,
                        word_value, long_value, long_long_value, pointer_value)),
      (llvm_varargs_proxy1(fixed0, signed_value, unsigned_byte, enum_value,
                           word_value, long_value, long_long_value,
                           pointer_value)));
  CHECK_SHAPE(
      2, golden2,
      (tasking_varargs2(fixed0, fixed1, signed_value, unsigned_byte,
                        enum_value, word_value, long_value, long_long_value,
                        pointer_value)),
      (llvm_varargs_proxy2(fixed0, fixed1, signed_value, unsigned_byte,
                           enum_value, word_value, long_value, long_long_value,
                           pointer_value)));
  CHECK_SHAPE(
      3, golden3,
      (tasking_varargs3(fixed0, fixed1, fixed2, signed_value, unsigned_byte,
                        enum_value, word_value, long_value, long_long_value,
                        pointer_value)),
      (llvm_varargs_proxy3(fixed0, fixed1, fixed2, signed_value, unsigned_byte,
                           enum_value, word_value, long_value, long_long_value,
                           pointer_value)));
  CHECK_SHAPE(
      4, golden4,
      (tasking_varargs4(fixed0, fixed1, fixed2, fixed3, signed_value,
                        unsigned_byte, enum_value, word_value, long_value,
                        long_long_value, pointer_value)),
      (llvm_varargs_proxy4(fixed0, fixed1, fixed2, fixed3, signed_value,
                           unsigned_byte, enum_value, word_value, long_value,
                           long_long_value, pointer_value)));
  CHECK_SHAPE(
      5, golden5,
      (tasking_varargs5(fixed0, fixed1, fixed2, fixed3, fixed4, signed_value,
                        unsigned_byte, enum_value, word_value, long_value,
                        long_long_value, pointer_value)),
      (llvm_varargs_proxy5(fixed0, fixed1, fixed2, fixed3, fixed4,
                           signed_value, unsigned_byte, enum_value, word_value,
                           long_value, long_long_value, pointer_value)));

#undef CHECK_SHAPE
}

static void run_varargs_stream(abi_u16 prefix_count, abi_u16 pointed_value,
                               abi_u32 golden) {
  volatile abi_u16 *pointer_value = &stream_cells[prefix_count];
  abi_u32 reference;
  abi_u32 actual;
  abi_u32 reverse;
  abi_u16 base = (abi_u16)(60U + prefix_count * 3U);

  *pointer_value = pointed_value;
  CALL_VARARGS_STREAM(tasking_varargs_stream, prefix_count, pointer_value,
                      reference);
  CALL_VARARGS_STREAM(llvm_varargs_stream_proxy, prefix_count, pointer_value,
                      actual);
  reverse = llvm_reverse_stream_entry_proxy(prefix_count, pointer_value);
  c166_test_check_u32(base + 1U, golden, reference);
  c166_test_check_u32(base + 2U, golden, actual);
  c166_test_check_u32(base + 3U, golden, reverse);
}

#define RUN_VARARGS_MATRIX(id, f0, f1, f2, f3, f4, signed_value, \
                           unsigned_byte, enum_value, word_value, long_value, \
                           long_long_value, pointed_value, g1, g2, g3, g4, g5) \
  run_varargs_matrix_vector(id, f0, f1, f2, f3, f4, signed_value, \
                            unsigned_byte, enum_value, word_value, long_value, \
                            long_long_value, pointed_value, g1, g2, g3, g4, g5);

#define RUN_VARARGS_STREAM(prefix_count, pointed_value, golden) \
  run_varargs_stream(prefix_count, pointed_value, golden);

void main(void) {
  c166_test_begin(268, 0x1660010cUL);
  ABI_VARARGS_MATRIX_VECTORS(RUN_VARARGS_MATRIX)
  ABI_VARARGS_STREAM_GOLDENS(RUN_VARARGS_STREAM)
  c166_test_finish();
  simulator_stop();
}

