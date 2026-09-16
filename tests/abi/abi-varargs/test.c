#include "c166_test.h"
#include "vectors.inc"
#include <stdarg.h>

C166_TEST_NOINLINE
unsigned long c166_test_entry(unsigned int fixed0, unsigned int fixed1, ...) {
  signed int signed_value;
  unsigned int unsigned_value;
  unsigned long long_value;
  volatile unsigned int *pointer_value;
  unsigned long result;
  va_list arguments;

  va_start(arguments, fixed1);
  signed_value = va_arg(arguments, signed int);
  unsigned_value = va_arg(arguments, unsigned int);
  long_value = va_arg(arguments, unsigned long);
  pointer_value = va_arg(arguments, volatile unsigned int *);
  va_end(arguments);

  result = long_value + ((unsigned long)fixed0 << 16);
  result ^= fixed1;
  result += signed_value;
  result ^= (unsigned long)unsigned_value << 16;
  result += *pointer_value;
  return result;
}

static volatile unsigned int cells[4];

static void run_varargs_vector(unsigned int vector_id, unsigned int fixed0,
                               unsigned int fixed1, signed char signed_value,
                               unsigned int unsigned_value,
                               unsigned long long_value,
                               unsigned int pointed_value,
                               unsigned long golden) {
  unsigned int index = vector_id - 1U;

  cells[index] = pointed_value;
  tap_is_u32(c166_test_entry(fixed0, fixed1, signed_value, unsigned_value,
                             long_value, &cells[index]),
             golden, "variadic argument vector");
}

#define RUN_VARARGS(id, fixed0, fixed1, signed_value, unsigned_value,       \
                    long_value, pointed_value, golden)                     \
  run_varargs_vector(id, fixed0, fixed1, signed_value, unsigned_value,      \
                     long_value, pointed_value, golden);

void main(void) {
  tap_plan(4U);
  ABI_VARARGS_VECTORS(RUN_VARARGS)
}
