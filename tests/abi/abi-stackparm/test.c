#include "types.h"

static abi_u16 C166_FAR far_values[] = {0x0000U, 0x0001U, 0x1357U,
                                        0xffffU};

static abi_u16 expected_mix(abi_u8 a, abi_u16 b, abi_u32 c,
                            const abi_u16 C166_FAR *p,
                            struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)(a * 19U) + b +
                   (abi_u16)((abi_u16)c * 3U) +
                   (abi_u16)((abi_u16)(c >> 16) * 7U) +
                   (abi_u16)(*p * 5U) + pair.lo +
                   (abi_u16)(pair.hi * 11U) + (abi_u16)(d * 13U));
}

static void run_mix(abi_u8 a, abi_u16 b, abi_u32 c, abi_u16 far_index,
                    abi_u16 lo, abi_u16 hi, abi_u16 d) {
  struct abi_stackparm_pair pair;
  const abi_u16 C166_FAR *p = &far_values[far_index];
  abi_u16 expected;

  pair.lo = lo;
  pair.hi = hi;
  expected = expected_mix(a, b, c, p, pair, d);
  tap_is_u32(stackparm_callee(a, b, c, p, pair, d), expected,
             "stackparm mixed callee");
  tap_is_u32(stackparm_direct(a, b, c, p, pair, d), expected,
             "direct stackparm mixed call");
  tap_is_u32(stackparm_indirect(stackparm_callee, a, b, c, p, pair, d),
             expected, "indirect stackparm mixed call");
}

static void check_float(const abi_float_words *actual,
                        const abi_float_words *expected, const char *name) {
  tap_is_u32(actual->words[0], expected->words[0], name);
  tap_is_u32(actual->words[1], expected->words[1], name);
}

static void check_double(const abi_double_words *actual,
                         const abi_double_words *expected, const char *name) {
  abi_u16 index;
  for (index = 0U; index != 4U; ++index)
    tap_is_u32(actual->words[index], expected->words[index], name);
}

static void run_floating(abi_u16 f1, abi_u16 f0, abi_u16 d3, abi_u16 d2,
                         abi_u16 d1, abi_u16 d0, abi_u16 tail) {
  abi_float_words float_input;
  abi_float_words float_direct;
  abi_float_words float_wrapper;
  abi_float_words float_indirect;
  abi_float_words float_routes[2];
  abi_double_words double_input;
  abi_double_words double_direct;
  abi_double_words double_wrapper;
  abi_double_words double_indirect;
  abi_double_words double_routes[2];

  float_input.words[0] = f1;
  float_input.words[1] = f0;
  double_input.words[0] = d3;
  double_input.words[1] = d2;
  double_input.words[2] = d1;
  double_input.words[3] = d0;

  float_direct.value = stackparm_float_callee(float_input.value, tail);
  float_wrapper.value = stackparm_float_direct(float_input.value, tail);
  float_indirect.value =
      stackparm_float_indirect(stackparm_float_callee, float_input.value, tail);
  double_direct.value = stackparm_double_callee(double_input.value, tail);
  double_wrapper.value = stackparm_double_direct(double_input.value, tail);
  double_indirect.value = stackparm_double_indirect(
      stackparm_double_callee, double_input.value, tail);
  stackparm_routes(
      stackparm_float_callee, stackparm_double_callee,
      (const abi_float_words C166_FAR *)&float_input,
      (const abi_double_words C166_FAR *)&double_input,
      (abi_float_words C166_FAR *)float_routes,
      (abi_double_words C166_FAR *)double_routes, tail);

  check_float(&float_direct, &float_input,
              "stackparm float callee");
  check_float(&float_wrapper, &float_input,
              "direct stackparm float call");
  check_float(&float_indirect, &float_input,
              "indirect stackparm float call");
  check_float(&float_routes[0], &float_input,
              "routed stackparm float call");
  check_float(&float_routes[1], &float_input,
              "routed indirect stackparm float call");
  check_double(&double_direct,
               &double_input, "stackparm double callee");
  check_double(&double_wrapper,
               &double_input, "direct stackparm double call");
  check_double(&double_indirect,
               &double_input, "indirect stackparm double call");
  check_double(&double_routes[0],
               &double_input, "routed stackparm double call");
  check_double(&double_routes[1],
               &double_input, "routed indirect stackparm double call");
}

void main(void) {
  tap_plan(192U);
  run_mix(0U, 0U, 0UL, 0U, 0U, 0U, 0U);
  run_mix(0xffU, 0xffffU, 0xffffffffUL, 3U, 0xffffU, 1U, 0x8000U);
  run_mix(0x81U, 0x2468U, 0x89abcdefUL, 2U, 0x55aaU, 0xaa55U, 0x1357U);
  run_mix(7U, 11U, 0x00010002UL, 1U, 13U, 17U, 19U);
  run_floating(0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U,
               0x0000U);
  run_floating(0x8000U, 0x0000U, 0x8000U, 0x0000U, 0x0000U, 0x0000U,
               0x0001U);
  run_floating(0x3f80U, 0x0000U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U,
               0x1357U);
  run_floating(0x7f80U, 0x0000U, 0x7ff0U, 0x0000U, 0x0000U, 0x0000U,
               0x8000U);
  run_floating(0x7fc1U, 0x2345U, 0x7ff8U, 0x1234U, 0x5678U, 0x9abcU,
               0xffffU);
  run_floating(0x0000U, 0x0001U, 0x0000U, 0x0000U, 0x0000U, 0x0001U,
               0x55aaU);
}
