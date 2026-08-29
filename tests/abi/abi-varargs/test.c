#include "c166-test-compat.h"
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
