#include "c166-test-result.h"
#include "types.h"

extern abi_u32 llvm_entry_proxy(
    abi_stackparm_function *function, abi_u8 a, abi_u16 b, abi_u32 c,
    const abi_u16 _far *p, struct abi_stackparm_pair pair, abi_u16 d);

extern abi_u16 ABI_STACKPARM tasking_stackparm_callee(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d);
extern float ABI_STACKPARM tasking_stackparm_float_callee(float value,
                                                          abi_u16 tail);
extern double ABI_STACKPARM tasking_stackparm_double_callee(double value,
                                                            abi_u16 tail);

/* The universal proxy slot at 10h:1300h transparently preserves R0. */
extern abi_u16 ABI_STACKPARM llvm_proxy_fn_0(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d);
extern float ABI_STACKPARM llvm_proxy_fn_1(float value, abi_u16 tail);
extern double ABI_STACKPARM llvm_proxy_fn_2(double value, abi_u16 tail);
extern void llvm_proxy_enum5(
    abi_stackparm_float_function *float_function,
    abi_stackparm_double_function *double_function,
    const abi_float_words _far *float_input,
    const abi_double_words _far *double_input,
    abi_float_words _far *float_output,
    abi_double_words _far *double_output, abi_u16 tail);

static abi_u16 far_values[] = {0x0000U, 0x0001U, 0x1357U, 0xffffU};

static abi_u16 expected_tasking(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)a + (abi_u16)(b * 3U) + (abi_u16)c +
                   (abi_u16)((abi_u16)(c >> 16) * 5U) +
                   (abi_u16)(*p * 7U) + (abi_u16)(pair.lo * 11U) +
                   (abi_u16)(pair.hi * 13U) + (abi_u16)(d * 17U));
}

static abi_u16 expected_llvm(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)(a * 19U) + b +
                   (abi_u16)((abi_u16)c * 3U) +
                   (abi_u16)((abi_u16)(c >> 16) * 7U) +
                   (abi_u16)(*p * 5U) + pair.lo +
                   (abi_u16)(pair.hi * 11U) + (abi_u16)(d * 13U));
}

static void run_vector(abi_u16 id, abi_u8 a, abi_u16 b, abi_u32 c,
                       abi_u16 far_index, abi_u16 lo, abi_u16 hi, abi_u16 d) {
  struct abi_stackparm_pair pair;
  const abi_u16 _far *p = &far_values[far_index];
  abi_u16 tasking_expected;
  abi_u32 pair_expected;

  pair.lo = lo;
  pair.hi = hi;
  tasking_expected = expected_tasking(a, b, c, p, pair, d);
  pair_expected = (abi_u32)tasking_expected |
                  ((abi_u32)tasking_expected << 16);

  c166_test_check_u32(id * 3U - 2U, tasking_expected,
                      tasking_stackparm_callee(a, b, c, p, pair, d));
  c166_test_check_u32(id * 3U - 1U, expected_llvm(a, b, c, p, pair, d),
                      llvm_proxy_fn_0(a, b, c, p, pair, d));
  c166_test_check_u32(id * 3U, pair_expected,
                      llvm_entry_proxy(tasking_stackparm_callee, a, b, c, p,
                                       pair, d));
}

static void check_float_words(abi_u16 base_case, abi_float_words *value,
                              abi_float_words *expected) {
  c166_test_check_u32(base_case, expected->words[0], value->words[0]);
  c166_test_check_u32((abi_u16)(base_case + 1U), expected->words[1],
                      value->words[1]);
}

static void check_double_words(abi_u16 base_case, abi_double_words *value,
                               abi_double_words *expected) {
  abi_u16 index;
  for (index = 0U; index != 4U; ++index)
    c166_test_check_u32((abi_u16)(base_case + index), expected->words[index],
                        value->words[index]);
}

static void run_special_vector(abi_u16 vector_id, abi_u16 f1, abi_u16 f0,
                               abi_u16 d3, abi_u16 d2, abi_u16 d1,
                               abi_u16 d0, abi_u16 tail) {
  abi_float_words float_input;
  abi_float_words expected_float;
  abi_float_words float_tasking;
  abi_float_words float_llvm_direct;
  abi_float_words float_llvm_indirect;
  abi_double_words double_input;
  abi_double_words expected_double;
  abi_double_words double_tasking;
  abi_double_words double_llvm_direct;
  abi_double_words double_llvm_indirect;
  abi_float_words float_llvm_to_tasking[2];
  abi_double_words double_llvm_to_tasking[2];
  abi_stackparm_float_function *float_target = llvm_proxy_fn_1;
  abi_stackparm_double_function *double_target = llvm_proxy_fn_2;
  abi_u16 base_case = (abi_u16)(100U + (vector_id - 1U) * 30U);

  float_input.words[0] = f1;
  float_input.words[1] = f0;
  expected_float = float_input;
  double_input.words[0] = d3;
  double_input.words[1] = d2;
  double_input.words[2] = d1;
  double_input.words[3] = d0;
  expected_double = double_input;

  float_tasking.value =
      tasking_stackparm_float_callee(float_input.value, tail);
  float_llvm_direct.value = llvm_proxy_fn_1(float_input.value, tail);
  float_llvm_indirect.value = float_target(float_input.value, tail);
  llvm_proxy_enum5(tasking_stackparm_float_callee,
                   tasking_stackparm_double_callee, &float_input,
                   &double_input, float_llvm_to_tasking,
                   double_llvm_to_tasking, tail);
  check_float_words(base_case, &float_tasking, &expected_float);
  check_float_words((abi_u16)(base_case + 2U), &float_llvm_direct,
                    &expected_float);
  check_float_words((abi_u16)(base_case + 4U), &float_llvm_indirect,
                    &expected_float);
  check_float_words((abi_u16)(base_case + 6U), &float_llvm_to_tasking[0],
                    &expected_float);
  check_float_words((abi_u16)(base_case + 8U), &float_llvm_to_tasking[1],
                    &expected_float);

  double_tasking.value =
      tasking_stackparm_double_callee(double_input.value, tail);
  double_llvm_direct.value = llvm_proxy_fn_2(double_input.value, tail);
  double_llvm_indirect.value = double_target(double_input.value, tail);
  check_double_words((abi_u16)(base_case + 10U), &double_tasking,
                     &expected_double);
  check_double_words((abi_u16)(base_case + 14U), &double_llvm_direct,
                     &expected_double);
  check_double_words((abi_u16)(base_case + 18U), &double_llvm_indirect,
                     &expected_double);
  check_double_words((abi_u16)(base_case + 22U),
                     &double_llvm_to_tasking[0], &expected_double);
  check_double_words((abi_u16)(base_case + 26U),
                     &double_llvm_to_tasking[1], &expected_double);
}

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  /* The freestanding ISS entry does not run the TASKING data initializer. */
  far_values[0] = 0x0000U;
  far_values[1] = 0x0001U;
  far_values[2] = 0x1357U;
  far_values[3] = 0xffffU;

  c166_test_begin(318, 0x1660013eUL);
  run_vector(1, 0U, 0U, 0UL, 0U, 0U, 0U, 0U);
  run_vector(2, 0xffU, 0xffffU, 0xffffffffUL, 3U, 0xffffU, 1U,
             0x8000U);
  run_vector(3, 0x81U, 0x2468U, 0x89abcdefUL, 2U, 0x55aaU, 0xaa55U,
             0x1357U);
  run_vector(4, 7U, 11U, 0x00010002UL, 1U, 13U, 17U, 19U);
  run_special_vector(1U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U,
                     0x0000U, 0x0000U);
  run_special_vector(2U, 0x8000U, 0x0000U, 0x8000U, 0x0000U, 0x0000U,
                     0x0000U, 0x0001U);
  run_special_vector(3U, 0x3f80U, 0x0000U, 0x3ff0U, 0x0000U, 0x0000U,
                     0x0000U, 0x1357U);
  run_special_vector(4U, 0x7f80U, 0x0000U, 0x7ff0U, 0x0000U, 0x0000U,
                     0x0000U, 0x8000U);
  run_special_vector(5U, 0x7fc1U, 0x2345U, 0x7ff8U, 0x1234U, 0x5678U,
                     0x9abcU, 0xffffU);
  run_special_vector(6U, 0x0000U, 0x0001U, 0x0000U, 0x0000U, 0x0000U,
                     0x0001U, 0x55aaU);
  c166_test_finish();
  simulator_stop();
}
