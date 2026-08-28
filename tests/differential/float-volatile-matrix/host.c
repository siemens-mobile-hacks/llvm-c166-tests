#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 volatile_f32_a[8];
extern volatile abi_u16 volatile_f32_b[8];
extern volatile abi_u16 volatile_f64_a[16];
extern volatile abi_u16 volatile_f64_b[16];
extern volatile abi_u16 volatile_observed[4];

extern void llvm_volatile_f32_eval_proxy(abi_u16 operation, abi_u16 index);
extern void llvm_volatile_f64_eval_proxy(abi_u16 operation, abi_u16 index);

void simulator_stop(void) {
  for (;;)
    ;
}

static void write_f32(volatile abi_u16 *base, abi_u16 index, abi_u32 bits) {
  base[index * 2U] = (abi_u16)(bits >> 16);
  base[index * 2U + 1U] = (abi_u16)bits;
}

static void write_f64(volatile abi_u16 *base, abi_u16 index, abi_u16 w3,
                      abi_u16 w2, abi_u16 w1, abi_u16 w0) {
  abi_u16 offset = index * 4U;
  base[offset] = w3;
  base[offset + 1U] = w2;
  base[offset + 2U] = w1;
  base[offset + 3U] = w0;
}

static void check_f32(abi_u16 case_id, abi_u32 expected,
                      volatile abi_u16 *base, abi_u16 index) {
  abi_u16 offset = index * 2U;
  abi_u32 actual = ((abi_u32)base[offset] << 16) | base[offset + 1U];
  c166_test_check_u32(case_id, expected, actual);
}

static void check_observed_f32(abi_u16 case_id, abi_u32 expected) {
  check_f32(case_id, expected, volatile_observed, 0U);
}

static void check_f64(abi_u16 case_id, abi_u16 e3, abi_u16 e2, abi_u16 e1,
                      abi_u16 e0, volatile abi_u16 *base, abi_u16 index) {
  abi_u16 offset = index * 4U;
  abi_u32 expected_hi = ((abi_u32)e3 << 16) | e2;
  abi_u32 expected_lo = ((abi_u32)e1 << 16) | e0;
  abi_u32 actual_hi = ((abi_u32)base[offset] << 16) | base[offset + 1U];
  abi_u32 actual_lo = ((abi_u32)base[offset + 2U] << 16) | base[offset + 3U];
  c166_test_check_u32(case_id, expected_hi, actual_hi);
  c166_test_check_u32((abi_u16)(case_id + 1U), expected_lo, actual_lo);
}

static void check_observed_f64(abi_u16 case_id, abi_u16 e3, abi_u16 e2,
                               abi_u16 e1, abi_u16 e0) {
  check_f64(case_id, e3, e2, e1, e0, volatile_observed, 0U);
}

static void run_f32_raw(abi_u16 id, abi_u32 bits) {
  abi_u16 case_id = (abi_u16)(0x0100U + (id - 1U) * 3U);
  abi_u16 index = (abi_u16)((id - 1U) & 3U);
  write_f32(volatile_f32_a, index, bits);
  write_f32(volatile_f32_b, index, 0x5a5aa5a5UL);
  llvm_volatile_f32_eval_proxy(0U, index);
  check_observed_f32(case_id, bits);
  llvm_volatile_f32_eval_proxy(1U, index);
  check_observed_f32((abi_u16)(case_id + 1U), bits);
  check_f32((abi_u16)(case_id + 2U), bits, volatile_f32_b, index);
}

static void run_f64_raw(abi_u16 id, abi_u16 w3, abi_u16 w2, abi_u16 w1,
                        abi_u16 w0) {
  abi_u16 case_id = (abi_u16)(0x1000U + (id - 1U) * 6U);
  abi_u16 index = (abi_u16)((id - 1U) & 3U);
  write_f64(volatile_f64_a, index, w3, w2, w1, w0);
  write_f64(volatile_f64_b, index, 0x5a5aU, 0xa5a5U, 0x1357U, 0x2468U);
  llvm_volatile_f64_eval_proxy(0U, index);
  check_observed_f64(case_id, w3, w2, w1, w0);
  llvm_volatile_f64_eval_proxy(1U, index);
  check_observed_f64((abi_u16)(case_id + 2U), w3, w2, w1, w0);
  check_f64((abi_u16)(case_id + 4U), w3, w2, w1, w0, volatile_f64_b,
            index);
}

static void reset_f32(abi_u16 index, abi_u32 a, abi_u32 b) {
  write_f32(volatile_f32_a, index, a);
  write_f32(volatile_f32_b, index, b);
}

static void run_f32_compound(abi_u16 id, abi_u32 a, abi_u32 b,
                             abi_u32 sum, abi_u32 product) {
  abi_u16 case_id = (abi_u16)(0x2000U + (id - 1U) * 10U);
  abi_u16 index = (abi_u16)((id - 1U) & 3U);
  reset_f32(index, a, b);
  llvm_volatile_f32_eval_proxy(2U, index);
  check_observed_f32(case_id++, sum);
  check_f32(case_id++, sum, volatile_f32_b, index);
  reset_f32(index, a, b);
  llvm_volatile_f32_eval_proxy(3U, index);
  check_observed_f32(case_id++, product);
  check_f32(case_id++, product, volatile_f32_b, index);
  reset_f32(index, a, b);
  llvm_volatile_f32_eval_proxy(4U, index);
  check_observed_f32(case_id++, sum);
  check_f32(case_id++, a, volatile_f32_a, index);
  check_f32(case_id++, b, volatile_f32_b, index);
  reset_f32(index, a, b);
  llvm_volatile_f32_eval_proxy(5U, index);
  check_observed_f32(case_id++, product);
  check_f32(case_id++, a, volatile_f32_a, index);
  check_f32(case_id, b, volatile_f32_b, index);
}

static void reset_f64(abi_u16 index, abi_u16 a3, abi_u16 a2, abi_u16 a1,
                      abi_u16 a0, abi_u16 b3, abi_u16 b2, abi_u16 b1,
                      abi_u16 b0) {
  write_f64(volatile_f64_a, index, a3, a2, a1, a0);
  write_f64(volatile_f64_b, index, b3, b2, b1, b0);
}

static void run_f64_compound(
    abi_u16 id, abi_u16 a3, abi_u16 a2, abi_u16 a1, abi_u16 a0, abi_u16 b3,
    abi_u16 b2, abi_u16 b1, abi_u16 b0, abi_u16 s3, abi_u16 s2, abi_u16 s1,
    abi_u16 s0, abi_u16 p3, abi_u16 p2, abi_u16 p1, abi_u16 p0) {
  abi_u16 case_id = (abi_u16)(0x3000U + (id - 1U) * 20U);
  abi_u16 index = (abi_u16)((id - 1U) & 3U);
  reset_f64(index, a3, a2, a1, a0, b3, b2, b1, b0);
  llvm_volatile_f64_eval_proxy(2U, index);
  check_observed_f64(case_id, s3, s2, s1, s0);
  check_f64((abi_u16)(case_id + 2U), s3, s2, s1, s0, volatile_f64_b,
            index);
  case_id += 4U;
  reset_f64(index, a3, a2, a1, a0, b3, b2, b1, b0);
  llvm_volatile_f64_eval_proxy(3U, index);
  check_observed_f64(case_id, p3, p2, p1, p0);
  check_f64((abi_u16)(case_id + 2U), p3, p2, p1, p0, volatile_f64_b,
            index);
  case_id += 4U;
  reset_f64(index, a3, a2, a1, a0, b3, b2, b1, b0);
  llvm_volatile_f64_eval_proxy(4U, index);
  check_observed_f64(case_id, s3, s2, s1, s0);
  check_f64((abi_u16)(case_id + 2U), a3, a2, a1, a0, volatile_f64_a,
            index);
  check_f64((abi_u16)(case_id + 4U), b3, b2, b1, b0, volatile_f64_b,
            index);
  case_id += 6U;
  reset_f64(index, a3, a2, a1, a0, b3, b2, b1, b0);
  llvm_volatile_f64_eval_proxy(5U, index);
  check_observed_f64(case_id, p3, p2, p1, p0);
  check_f64((abi_u16)(case_id + 2U), a3, a2, a1, a0, volatile_f64_a,
            index);
  check_f64((abi_u16)(case_id + 4U), b3, b2, b1, b0, volatile_f64_b,
            index);
}

#define RUN_F32_RAW(id, bits) run_f32_raw(id, bits);
#define RUN_F64_RAW(id, w3, w2, w1, w0) run_f64_raw(id, w3, w2, w1, w0);
#define RUN_F32_COMPOUND(id, a, b, sum, product)                            \
  run_f32_compound(id, a, b, sum, product);
#define RUN_F64_COMPOUND(id, a3, a2, a1, a0, b3, b2, b1, b0, s3, s2, s1,  \
                         s0, p3, p2, p1, p0)                               \
  run_f64_compound(id, a3, a2, a1, a0, b3, b2, b1, b0, s3, s2, s1, s0,   \
                   p3, p2, p1, p0);

void main(void) {
  c166_test_begin(301, 0x1660012dUL);
  ABI_VOLATILE_F32_RAW_VECTORS(RUN_F32_RAW)
  ABI_VOLATILE_F64_RAW_VECTORS(RUN_F64_RAW)
  ABI_VOLATILE_F32_COMPOUND_VECTORS(RUN_F32_COMPOUND)
  ABI_VOLATILE_F64_COMPOUND_VECTORS(RUN_F64_COMPOUND)
  llvm_volatile_f32_eval_proxy(6U, 0U);
  check_observed_f32(0x4000U, 0x00000000UL);
  llvm_volatile_f32_eval_proxy(6U, 1U);
  check_observed_f32(0x4001U, 0x00000000UL);
  c166_test_finish();
  simulator_stop();
}

