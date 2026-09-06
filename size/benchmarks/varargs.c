#include <stdarg.h>

typedef unsigned short u16;
typedef unsigned long u32;

u32 size_varargs(unsigned int count, ...) {
  va_list arguments;
  u32 sum = 0;
  va_start(arguments, count);
  while (count != 0u) {
    sum += (u16)va_arg(arguments, unsigned int);
    --count;
  }
  va_end(arguments);
  return sum;
}
