#include "c166_test.h"
#include <stdarg.h>

typedef unsigned int u16;

struct bool_record {
  unsigned char prefix;
  C166_BOOL value;
  u16 tail;
};

static volatile u16 bool_input;

C166_NOINLINE static C166_BOOL identity(C166_BOOL value) { return value; }

C166_NOINLINE static u16 read_promoted_bool(u16 tag, ...) {
  va_list args;
  int value;

  va_start(args, tag);
  value = va_arg(args, int);
  va_end(args);
  return (u16)value;
}

C166_NOINLINE static u16 evaluate(u16 input) {
  struct bool_record record;
  C166_BOOL value;
  u16 result = 0;

  bool_input = input;
  value = C166_TO_BOOL(bool_input);
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

void main(void) {
  tap_plan(4);
  tap_ok(evaluate(0) == 0x3f, "zero converts to false");
  tap_ok(evaluate(1) == 0x3f, "one converts to true");
  tap_ok(evaluate(2) == 0x3f, "nonzero converts to true");
  tap_ok(evaluate(0xffffU) == 0x3f, "all bits convert to true");
}
