#include <stdarg.h>

typedef unsigned int u16;

struct bool_record {
  unsigned char prefix;
  _Bool value;
  u16 tail;
};

volatile u16 bool_input;

__attribute__((noinline)) static _Bool identity(_Bool value) { return value; }

__attribute__((noinline)) static u16 read_promoted_bool(u16 tag, ...) {
  va_list args;
  va_start(args, tag);
  int value = va_arg(args, int);
  va_end(args);
  return (u16)value;
}

__attribute__((noinline)) u16 llvm_bool_eval(u16 input) {
  struct bool_record record;
  _Bool value;
  u16 result = 0;

  bool_input = input;
  value = bool_input;
  record.prefix = 0x5a;
  record.value = value;
  record.tail = 0xa55a;

  result |= (u16)(value == (input != 0)) << 0;
  result |= (u16)(identity(value) == value) << 1;
  result |= (u16)(identity(!value) == !value) << 2;
  result |= (u16)(record.value == value) << 3;
  result |= (u16)(record.prefix == 0x5a && record.tail == 0xa55a) << 4;
  result |= (u16)(read_promoted_bool(0, value) == (u16)value) << 5;
  return result;
}
