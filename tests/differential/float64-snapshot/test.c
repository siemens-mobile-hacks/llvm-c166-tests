#include "c166_test.h"

extern void snapshot_initialize(double *);
extern void snapshot_mutate(double *);

/* The separate translation unit keeps the mutation opaque to both compilers. */
unsigned int snapshot_eval(unsigned int mode) {
  double value;
  double old;
  double first;
  snapshot_initialize(&value);
  old = value;
  if (mode == 0U) {
    snapshot_mutate(&value);
    return old + old == 2.0;
  }
  if (mode == 1U) {
    first = old + old;
    snapshot_mutate(&value);
    return first + old == 3.0;
  }
  if (mode == 2U) {
    snapshot_mutate(&value);
    return old < value;
  }
  snapshot_mutate(&value);
  value = old * old + value;
  return value == 3.0;
}

void main(void) {
  unsigned int mode;

  tap_plan(4U);
  for (mode = 0U; mode != 4U; ++mode)
    tap_is_u32(snapshot_eval(mode), 1U, "binary64 snapshot");
}
