#include "c166_test.h"

C166_NOINLINE C166_SECTION(".seeded_f64_to_f32")
float seeded_f64_to_f32(double value) { return (float)value; }
