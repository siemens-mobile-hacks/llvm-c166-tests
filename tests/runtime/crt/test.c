#include "c166_test.h"

static volatile unsigned int initialized = 0x1234U;
static volatile unsigned int zeroed;

void main(void) {
  tap_plan(2);
  tap_ok(initialized == 0x1234U, ".data initialized");
  tap_ok(zeroed == 0, ".bss zeroed");
}
