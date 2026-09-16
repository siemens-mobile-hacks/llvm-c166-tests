#include "c166_test.h"

void c166_asc0_init(void);
void c166_asc0_putc(char value);

static unsigned int next_test = 1;
static unsigned int planned_tests;

#if defined(C166_TEST_TASKING)
void c166_test_halt(void) {
  for (;;)
    ;
}
#else
void c166_test_halt(void);
#endif

static void write_string(const char *text) {
  while (*text)
    c166_asc0_putc(*text++);
}

static void write_unsigned(unsigned int value) {
  char digits[5];
  unsigned int count = 0;

  do {
    digits[count++] = '0' + value % 10U;
    value /= 10U;
  } while (value);
  while (count)
    c166_asc0_putc(digits[--count]);
}

static void write_hex32(unsigned long value) {
  static const char digits[] = "0123456789abcdef";
  unsigned int shift = 32;

  write_string("0x");
  while (shift) {
    shift -= 4;
    c166_asc0_putc(digits[(value >> shift) & 0xfUL]);
  }
}

static void finish_test(void) {
  if (next_test > planned_tests)
    c166_test_halt();
}

static void write_result(int condition, const char *name) {
  write_string(condition ? "ok " : "not ok ");
  write_unsigned(next_test++);
  write_string(" - ");
  write_string(name);
  c166_asc0_putc('\n');
}

void tap_plan(unsigned int count) {
  c166_asc0_init();
  planned_tests = count;
  write_string("TAP version 13\n1..");
  write_unsigned(count);
  c166_asc0_putc('\n');
}

void tap_ok(int condition, const char *name) {
  write_result(condition, name);
  finish_test();
}

void tap_is_u32(unsigned long actual, unsigned long expected,
                const char *name) {
  int equal = actual == expected;

  write_result(equal, name);
  if (!equal) {
    write_string("# expected: ");
    write_hex32(expected);
    write_string("\n# actual:   ");
    write_hex32(actual);
    c166_asc0_putc('\n');
  }
  finish_test();
}
