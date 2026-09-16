#include "c166_test.h"
#include "matrix-common.h"
#include "vectors.inc"

volatile abi_u16 cells[4];
volatile abi_u16 stream_cells[8];

static void run_matrix_vector(
    abi_u16 vector_id, abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
    abi_u16 fixed3, abi_u16 fixed4, signed char signed_value,
    unsigned char unsigned_byte, enum matrix_enum enum_value,
    abi_u16 word_value, abi_u32 long_value,
    abi_u32_second second_long_value, abi_u16 pointed_value,
    abi_u32 golden1, abi_u32 golden2, abi_u32 golden3, abi_u32 golden4,
    abi_u32 golden5) {
  volatile abi_u16 *pointer_value = &cells[vector_id - 1U];

  cells[vector_id - 1U] = pointed_value;

#define CHECK_SHAPE(shape, golden, direct_call)                                \
  do {                                                                         \
    tap_is_u32((direct_call), (golden), "variadic shape direct call");        \
    tap_is_u32(varargs_dispatch(                                                \
                   (shape), fixed0, fixed1, fixed2, fixed3, fixed4,            \
                   signed_value, unsigned_byte, enum_value, word_value,        \
                   long_value, second_long_value, pointer_value),              \
               (golden), "variadic shape dispatcher call");                  \
  } while (0)

  CHECK_SHAPE(1, golden1,
              varargs1(fixed0, signed_value, unsigned_byte, enum_value,
                       word_value, long_value, second_long_value, pointer_value));
  CHECK_SHAPE(2, golden2,
              varargs2(fixed0, fixed1, signed_value, unsigned_byte, enum_value,
                       word_value, long_value, second_long_value, pointer_value));
  CHECK_SHAPE(3, golden3,
              varargs3(fixed0, fixed1, fixed2, signed_value, unsigned_byte,
                       enum_value, word_value, long_value, second_long_value,
                       pointer_value));
  CHECK_SHAPE(4, golden4,
              varargs4(fixed0, fixed1, fixed2, fixed3, signed_value,
                       unsigned_byte, enum_value, word_value, long_value,
                       second_long_value, pointer_value));
  CHECK_SHAPE(5, golden5,
              varargs5(fixed0, fixed1, fixed2, fixed3, fixed4, signed_value,
                       unsigned_byte, enum_value, word_value, long_value,
                       second_long_value, pointer_value));

#undef CHECK_SHAPE
}

static void run_stream(abi_u16 prefix_count, abi_u16 pointed_value,
                       abi_u32 golden) {
  volatile abi_u16 *pointer_value = &stream_cells[prefix_count];
  abi_u32 direct;

  *pointer_value = pointed_value;
  CALL_VARARGS_STREAM(varargs_stream, prefix_count, pointer_value, direct);
  tap_is_u32(direct, golden, "variadic stream direct call");
  tap_is_u32(varargs_stream_call(prefix_count, pointer_value), golden,
             "variadic stream separate caller");
}

#define RUN_MATRIX(id, f0, f1, f2, f3, f4, signed_value, unsigned_byte,      \
                   enum_value, word_value, long_value, second_long_value,     \
                   pointed_value, g1, g2, g3, g4, g5)                         \
  run_matrix_vector(id, f0, f1, f2, f3, f4, signed_value, unsigned_byte,      \
                    enum_value, word_value, long_value, second_long_value,     \
                    pointed_value, g1, g2, g3, g4, g5);

#define RUN_STREAM(prefix_count, pointed_value, golden) \
  run_stream(prefix_count, pointed_value, golden);

void main(void) {
  tap_plan(56U);
  ABI_VARARGS_MATRIX_VECTORS(RUN_MATRIX)
  ABI_VARARGS_STREAM_GOLDENS(RUN_STREAM)
}
