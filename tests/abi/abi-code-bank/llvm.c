typedef unsigned int u16;

typedef union {
  float value;
  u16 words[2];
} float_words;

typedef union {
  double value;
  u16 words[4];
} double_words;

#define NOINLINE __attribute__((noinline))
#define BANK1 __attribute__((noinline, c166_bank(1)))
#define BANK2 __attribute__((noinline, c166_bank(2)))

typedef u16 __attribute__((c166_bank(1))) bank1_fn5(u16, u16, u16, u16,
                                                     u16);
typedef u16 __attribute__((c166_bank(2))) bank2_fn5(u16, u16, u16, u16,
                                                     u16);
typedef float __attribute__((c166_bank(1))) bank1_float_fn(float, u16);
typedef float __attribute__((c166_bank(2))) bank2_float_fn(float, u16);
typedef double __attribute__((c166_bank(1))) bank1_double_fn(double, u16);
typedef double __attribute__((c166_bank(2))) bank2_double_fn(double, u16);

static volatile u16 input_seed = 0x1234U;

// Logical bank state maintained by the ISS-only __banksw contract veneer.
// A real platform implementation replaces this with its hardware register.
volatile u16 banksw_active_bank;
volatile u16 banksw_call_count;
volatile u16 banksw_restore_count;
volatile u16 banksw_depth;
volatile u16 banksw_max_depth;
volatile u16 banksw_failure;
static volatile float_words bank_float_observed;
static volatile double_words bank_double_observed;

struct bank_float_vector {
  u16 f1;
  u16 f0;
  u16 d3;
  u16 d2;
  u16 d1;
  u16 d0;
  u16 tail;
};

static const struct bank_float_vector bank_float_vectors[] = {
    {0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U, 0x0000U},
    {0x8000U, 0x0000U, 0x8000U, 0x0000U, 0x0000U, 0x0000U, 0x0001U},
    {0x3f80U, 0x0000U, 0x3ff0U, 0x0000U, 0x0000U, 0x0000U, 0x1357U},
    {0x7f80U, 0x0000U, 0x7ff0U, 0x0000U, 0x0000U, 0x0000U, 0x8000U},
    {0x7fc1U, 0x2345U, 0x7ff8U, 0x1234U, 0x5678U, 0x9abcU, 0xffffU},
    {0x0000U, 0x0001U, 0x0000U, 0x0000U, 0x0000U, 0x0001U, 0x55aaU},
};

static u16 mix(u16 tag, u16 a, u16 b, u16 c, u16 d, u16 e) {
  return (u16)(tag ^ a ^ (u16)(b << 1) ^ (u16)(c << 3) ^
               (u16)(d << 5) ^ (u16)(e << 7));
}

BANK1 u16 bank1_target(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return mix(0x1101U, a, b, c, d, e);
}

BANK2 u16 bank2_target(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return mix(0x2202U, a, b, c, d, e);
}

NOINLINE u16 plain_target(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return mix(0x3303U, a, b, c, d, e);
}

BANK1 u16 bank1_same_direct(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return bank1_target(a, b, c, d, e);
}

BANK1 u16 bank1_cross_direct(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return bank2_target(a, b, c, d, e);
}

BANK2 u16 bank2_reverse_direct(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return bank1_target(a, b, c, d, e);
}

BANK1 u16 bank1_same_indirect(bank1_fn5 *target, u16 a, u16 b, u16 c,
                              u16 d, u16 e) {
  return target(a, b, c, d, e);
}

BANK1 u16 bank1_cross_indirect(bank2_fn5 *target, u16 a, u16 b, u16 c,
                               u16 d, u16 e) {
  return target(a, b, c, d, e);
}

BANK1 u16 bank1_to_plain(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return plain_target(a, b, c, d, e);
}

BANK1 float bank1_float_target(float value, u16 tail) {
  (void)tail;
  return value;
}

BANK2 float bank2_float_target(float value, u16 tail) {
  (void)tail;
  return value;
}

BANK1 double bank1_double_target(double value, u16 tail) {
  (void)tail;
  return value;
}

BANK2 double bank2_double_target(double value, u16 tail) {
  (void)tail;
  return value;
}

BANK1 float bank1_float_same(float value, u16 tail) {
  return bank1_float_target(value, tail);
}

BANK1 float bank1_float_cross(float value, u16 tail) {
  return bank2_float_target(value, tail);
}

BANK1 double bank1_double_same(double value, u16 tail) {
  return bank1_double_target(value, tail);
}

BANK1 double bank1_double_cross(double value, u16 tail) {
  return bank2_double_target(value, tail);
}

BANK1 float bank1_float_indirect_same(bank1_float_fn *target, float value,
                                      u16 tail) {
  return target(value, tail);
}

BANK1 float bank1_float_indirect_cross(bank2_float_fn *target, float value,
                                       u16 tail) {
  return target(value, tail);
}

BANK1 double bank1_double_indirect_same(bank1_double_fn *target, double value,
                                        u16 tail) {
  return target(value, tail);
}

BANK1 double bank1_double_indirect_cross(bank2_double_fn *target,
                                         double value, u16 tail) {
  return target(value, tail);
}

static NOINLINE u16 observe_bank_float(float value, u16 expected1,
                                       u16 expected0) {
  bank_float_observed.value = value;
  return (u16)(bank_float_observed.words[0] == expected1 &&
               bank_float_observed.words[1] == expected0);
}

static NOINLINE u16 observe_bank_double(double value, u16 expected3,
                                        u16 expected2, u16 expected1,
                                        u16 expected0) {
  bank_double_observed.value = value;
  return (u16)(bank_double_observed.words[0] == expected3 &&
               bank_double_observed.words[1] == expected2 &&
               bank_double_observed.words[2] == expected1 &&
               bank_double_observed.words[3] == expected0);
}

static NOINLINE u16 run_banked_floating(void) {
  u16 failures = 0U;
  u16 index;

  for (index = 0U; index !=
                       (u16)(sizeof(bank_float_vectors) /
                             sizeof(bank_float_vectors[0]));
       ++index) {
    const struct bank_float_vector *vector = &bank_float_vectors[index];
    volatile float_words float_input;
    volatile double_words double_input;

    float_input.words[0] = vector->f1;
    float_input.words[1] = vector->f0;
    double_input.words[0] = vector->d3;
    double_input.words[1] = vector->d2;
    double_input.words[2] = vector->d1;
    double_input.words[3] = vector->d0;

    if (!observe_bank_float(
            bank1_float_target(float_input.value, vector->tail), vector->f1,
            vector->f0))
      ++failures;
    if (!observe_bank_float(
            bank2_float_target(float_input.value, vector->tail), vector->f1,
            vector->f0))
      ++failures;
    if (!observe_bank_float(
            bank1_float_same(float_input.value, vector->tail), vector->f1,
            vector->f0))
      ++failures;
    if (!observe_bank_float(
            bank1_float_cross(float_input.value, vector->tail), vector->f1,
            vector->f0))
      ++failures;
    if (!observe_bank_float(
            bank1_float_indirect_same(bank1_float_target, float_input.value,
                                      vector->tail),
            vector->f1, vector->f0))
      ++failures;
    if (!observe_bank_float(
            bank1_float_indirect_cross(bank2_float_target, float_input.value,
                                       vector->tail),
            vector->f1, vector->f0))
      ++failures;

    if (!observe_bank_double(
            bank1_double_target(double_input.value, vector->tail), vector->d3,
            vector->d2, vector->d1, vector->d0))
      ++failures;
    if (!observe_bank_double(
            bank2_double_target(double_input.value, vector->tail), vector->d3,
            vector->d2, vector->d1, vector->d0))
      ++failures;
    if (!observe_bank_double(
            bank1_double_same(double_input.value, vector->tail), vector->d3,
            vector->d2, vector->d1, vector->d0))
      ++failures;
    if (!observe_bank_double(
            bank1_double_cross(double_input.value, vector->tail), vector->d3,
            vector->d2, vector->d1, vector->d0))
      ++failures;
    if (!observe_bank_double(
            bank1_double_indirect_same(bank1_double_target,
                                       double_input.value, vector->tail),
            vector->d3, vector->d2, vector->d1, vector->d0))
      ++failures;
    if (!observe_bank_double(
            bank1_double_indirect_cross(bank2_double_target,
                                        double_input.value, vector->tail),
            vector->d3, vector->d2, vector->d1, vector->d0))
      ++failures;
  }
  return failures;
}

BANK1 u16 bank1_deep_leaf(u16 a, u16 b, u16 c, u16 d, u16 e) {
  if (banksw_active_bank != 1U)
    return 0xd101U;
  return mix(0x4414U, a, b, c, d, e);
}

BANK2 u16 bank2_nested_chain(u16 a, u16 b, u16 c, u16 d, u16 e) {
  u16 result;
  if (banksw_active_bank != 2U)
    return 0xd202U;
  result = bank1_deep_leaf(a, b, c, d, e);
  if (banksw_active_bank != 2U)
    return 0xd203U;
  return result;
}

BANK1 u16 bank1_nested_chain(u16 a, u16 b, u16 c, u16 d, u16 e) {
  u16 result;
  if (banksw_active_bank != 1U)
    return 0xd301U;
  result = bank2_nested_chain(a, b, c, d, e);
  if (banksw_active_bank != 1U)
    return 0xd302U;
  return result;
}

NOINLINE u16 code_bank_nested_driver(void) {
  return bank1_nested_chain(0x0101U, 0x0202U, 0x0303U, 0x0404U, 0x0505U);
}

extern u16 code_bank_state_probe(void);

u16 llvm_entry(void) {
  u16 a = input_seed;
  u16 b = (u16)(a ^ 0x0102U);
  u16 c = (u16)(a + 0x0304U);
  u16 d = (u16)(a ^ 0x0506U);
  u16 e = (u16)(a + 0x0708U);
  u16 expected1 = mix(0x1101U, a, b, c, d, e);
  u16 expected2 = mix(0x2202U, a, b, c, d, e);
  u16 expected_plain = mix(0x3303U, a, b, c, d, e);
  u16 failures = 0;

  // bank0 -> bank1 and bank0 -> bank2.
  if (bank1_target(a, b, c, d, e) != expected1)
    failures |= 1U;
  if (bank2_target(a, b, c, d, e) != expected2)
    failures |= 2U;

  // bank0 -> bank1 -> bank1, then bank0 -> bank1 -> bank2.
  if (bank1_same_direct(a, b, c, d, e) != expected1)
    failures |= 4U;
  if (bank1_cross_direct(a, b, c, d, e) != expected2)
    failures |= 8U;

  // bank0 -> bank2 -> bank1.
  if (bank2_reverse_direct(a, b, c, d, e) != expected1)
    failures |= 16U;

  // Same-bank indirect calls use __icall; cross-bank indirect calls use
  // __banksw.  Both still reserve the hidden word before normal stack args.
  if (bank1_same_indirect(bank1_target, a, b, c, d, e) != expected1)
    failures |= 32U;
  if (bank1_cross_indirect(bank2_target, a, b, c, d, e) != expected2)
    failures |= 64U;

  // A banked caller reaching bank zero is an ordinary far call with no
  // __banksw protocol and no hidden bank word in the unbanked callee.
  if (bank1_to_plain(a, b, c, d, e) != expected_plain)
    failures |= 128U;

  if (run_banked_floating() != 0U)
    failures |= 0x2000U;

  // Isolate one exact nested state-machine trace: bank 0 -> 1 -> 2 -> 1.
  // The two inner returns restore bank 2 and then bank 1; the outer call has
  // current bank zero, so the manual says no final restoration is required.
  banksw_active_bank = 0U;
  banksw_call_count = 0U;
  banksw_restore_count = 0U;
  banksw_depth = 0U;
  banksw_max_depth = 0U;
  banksw_failure = 0U;
  if (code_bank_state_probe() !=
      mix(0x4414U, 0x0101U, 0x0202U, 0x0303U, 0x0404U, 0x0505U))
    failures |= 0x0100U;
  if (banksw_call_count != 3U || banksw_restore_count != 2U)
    failures |= 0x0200U;
  if (banksw_depth != 0U || banksw_max_depth != 3U)
    failures |= 0x0400U;
  if (banksw_active_bank != 1U)
    failures |= 0x0800U;
  if (banksw_failure != 0U)
    failures |= 0x1000U;

  return failures ? failures : 0xb166U;
}
