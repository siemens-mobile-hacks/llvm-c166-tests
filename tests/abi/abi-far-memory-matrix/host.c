#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern volatile struct abi_far_arena far_page1_arena;
extern volatile struct abi_far_arena far_page4_arena;

extern abi_u32 llvm_far_apply_proxy(abi_u16 seed,
                                    volatile struct abi_far_arena *arena);
extern volatile abi_u8 *llvm_far_advance_proxy(
    volatile struct abi_far_arena *arena, abi_u16 offset);
extern abi_u32 llvm_far_reverse_apply_proxy(
    abi_u16 seed, volatile struct abi_far_arena *arena);
extern volatile abi_u8 *llvm_far_reverse_advance_proxy(
    volatile struct abi_far_arena *arena, abi_u16 offset);
extern abi_u32 llvm_far_advance_bits_proxy(
    volatile struct abi_far_arena *arena, abi_u16 offset);
extern abi_u32 tasking_far_advance_bits_proxy(
    volatile struct abi_far_arena *arena, abi_u16 offset);
extern abi_u32 llvm_far_reverse_advance_bits_proxy(
    volatile struct abi_far_arena *arena, abi_u16 offset);
extern abi_u32 llvm_far_retreat_bits_proxy(
    volatile abi_u8 *one_past, abi_u16 offset);
extern abi_u32 tasking_far_retreat_bits_proxy(
    volatile abi_u8 *one_past, abi_u16 offset);
extern abi_u32 llvm_far_reverse_retreat_bits_proxy(
    volatile abi_u8 *one_past, abi_u16 offset);
extern abi_u16 llvm_far_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_not_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_not_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_less_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_less_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_less_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_less_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_greater_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_greater_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_greater_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 tasking_far_greater_equal_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_s16 llvm_far_difference_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_s16 tasking_far_difference_raw_proxy(
    abi_u16 lhs_offset, abi_u16 lhs_page,
    abi_u16 rhs_offset, abi_u16 rhs_page);
extern abi_u16 llvm_far_is_null_raw_proxy(abi_u16 offset, abi_u16 page);
extern abi_u16 tasking_far_is_null_raw_proxy(abi_u16 offset, abi_u16 page);

void simulator_stop(void) {
  for (;;)
    ;
}

static void fill_arena(volatile struct abi_far_arena *arena, abi_u16 seed) {
  volatile abi_u8 *bytes = (volatile abi_u8 *)arena;
  abi_u16 index;

  for (index = 0; index != 48U; ++index)
    bytes[index] = (abi_u8)(seed + index * 0x25U + 0x5bU);
}

static abi_u32 arena_digest(volatile struct abi_far_arena *arena) {
  volatile abi_u8 *bytes = (volatile abi_u8 *)arena;
  abi_u16 low = 0x4a39U;
  abi_u16 high = 0x6d2bU;
  abi_u16 index;

  for (index = 0; index != 48U; ++index) {
    low = low * 33U + bytes[index] + index;
    high = high * 257U;
    high ^= ((abi_u16)bytes[index] << 8) | index;
  }
  return ((abi_u32)high << 16) | low;
}

static void check_pair(abi_u16 base_case, abi_u32 expected_hash,
                       abi_u32 actual_hash,
                       volatile struct abi_far_arena *expected,
                       volatile struct abi_far_arena *actual) {
  c166_test_check_u32(base_case + 1U, expected_hash, actual_hash);
  c166_test_check_u32(base_case + 2U, arena_digest(expected),
                      arena_digest(actual));
}

static void check_page_boundary(abi_u16 base_case,
                                volatile struct abi_far_arena *arena,
                                abi_u32 expected_base) {
  /* Forming one-past is valid C; it is observed as raw words, never read. */
  volatile abi_u8 *one_past =
      (volatile abi_u8 *)arena + sizeof(struct abi_far_arena);

#define CHECK_FORWARD_BITS(index, offset)                                  \
  c166_test_check_u32(base_case + (index - 1U) * 3U + 1U,                  \
      expected_base + offset, tasking_far_advance_bits_proxy(arena, offset)); \
  c166_test_check_u32(base_case + (index - 1U) * 3U + 2U,                  \
      expected_base + offset, llvm_far_advance_bits_proxy(arena, offset)); \
  c166_test_check_u32(base_case + (index - 1U) * 3U + 3U,                  \
      expected_base + offset,                                              \
      llvm_far_reverse_advance_bits_proxy(arena, offset));

  ABI_FAR_MEMORY_FORWARD_OFFSETS(CHECK_FORWARD_BITS)
#undef CHECK_FORWARD_BITS

#define CHECK_RETREAT_BITS(index, offset)                                  \
  c166_test_check_u32(base_case + ABI_FAR_MEMORY_FORWARD_CASES +           \
      (index - 1U) * 3U + 1U, expected_base + 48UL - offset,               \
      tasking_far_retreat_bits_proxy(one_past, offset));                    \
  c166_test_check_u32(base_case + ABI_FAR_MEMORY_FORWARD_CASES +           \
      (index - 1U) * 3U + 2U, expected_base + 48UL - offset,               \
      llvm_far_retreat_bits_proxy(one_past, offset));                       \
  c166_test_check_u32(base_case + ABI_FAR_MEMORY_FORWARD_CASES +           \
      (index - 1U) * 3U + 3U, expected_base + 48UL - offset,               \
      llvm_far_reverse_retreat_bits_proxy(one_past, offset));

  ABI_FAR_MEMORY_RETREAT_OFFSETS(CHECK_RETREAT_BITS)
#undef CHECK_RETREAT_BITS
}

static void check_far_relations(abi_u16 base_case) {
#define CHECK_BINARY(index, expected, function, lhs_offset, lhs_page,       \
                     rhs_offset, rhs_page)                                 \
  c166_test_check_u32(base_case + (index - 1U) * 2U + 1U, expected,        \
      tasking_##function##_raw_proxy(lhs_offset, lhs_page,                  \
                                      rhs_offset, rhs_page));               \
  c166_test_check_u32(base_case + (index - 1U) * 2U + 2U, expected,        \
      llvm_##function##_raw_proxy(lhs_offset, lhs_page,                     \
                                   rhs_offset, rhs_page));

  /* All default _far relations compare only the stored low offset word. */
#define CHECK_RELATION_PAIR(index, lhs_offset, lhs_page, rhs_offset, rhs_page) \
  CHECK_BINARY((index - 1U) * 6U + 1U, lhs_offset == rhs_offset, far_equal,    \
               lhs_offset, lhs_page, rhs_offset, rhs_page)                    \
  CHECK_BINARY((index - 1U) * 6U + 2U, lhs_offset != rhs_offset,              \
               far_not_equal, lhs_offset, lhs_page, rhs_offset, rhs_page)     \
  CHECK_BINARY((index - 1U) * 6U + 3U, lhs_offset < rhs_offset, far_less,     \
               lhs_offset, lhs_page, rhs_offset, rhs_page)                    \
  CHECK_BINARY((index - 1U) * 6U + 4U, lhs_offset <= rhs_offset,              \
               far_less_equal, lhs_offset, lhs_page, rhs_offset, rhs_page)    \
  CHECK_BINARY((index - 1U) * 6U + 5U, lhs_offset > rhs_offset, far_greater,  \
               lhs_offset, lhs_page, rhs_offset, rhs_page)                    \
  CHECK_BINARY((index - 1U) * 6U + 6U, lhs_offset >= rhs_offset,              \
               far_greater_equal, lhs_offset, lhs_page, rhs_offset, rhs_page)

  CHECK_RELATION_PAIR(1U, 0x0000U, 0U, 0x0000U, 0U)
  CHECK_RELATION_PAIR(2U, 0x0000U, 1U, 0x0000U, 4U)
  CHECK_RELATION_PAIR(3U, 0x0000U, 4U, 0x0001U, 1U)
  CHECK_RELATION_PAIR(4U, 0x0001U, 1U, 0x0000U, 4U)
  CHECK_RELATION_PAIR(5U, 0x3fd0U, 1U, 0x3fd0U, 4U)
  CHECK_RELATION_PAIR(6U, 0x3fd0U, 4U, 0x3fd1U, 1U)
  CHECK_RELATION_PAIR(7U, 0x3fffU, 1U, 0x3ffeU, 4U)
  CHECK_RELATION_PAIR(8U, 0x4000U, 4U, 0x3fffU, 1U)
#undef CHECK_RELATION_PAIR
#undef CHECK_BINARY

#define CHECK_DIFFERENCE(index, expected, lhs_offset, lhs_page, rhs_offset,  \
                         rhs_page)                                          \
  c166_test_check_u32(base_case + 96U + (index - 1U) * 2U + 1U, expected,   \
      (abi_u16)tasking_far_difference_raw_proxy(                            \
          lhs_offset, lhs_page, rhs_offset, rhs_page));                     \
  c166_test_check_u32(base_case + 96U + (index - 1U) * 2U + 2U, expected,   \
      (abi_u16)llvm_far_difference_raw_proxy(                               \
          lhs_offset, lhs_page, rhs_offset, rhs_page));

  CHECK_DIFFERENCE(1U, 0U, 0x0000U, 1U, 0x0000U, 4U)
  CHECK_DIFFERENCE(2U, 0xffffU, 0x0000U, 4U, 0x0002U, 1U)
  CHECK_DIFFERENCE(3U, 1U, 0x0002U, 1U, 0x0000U, 4U)
  CHECK_DIFFERENCE(4U, 0xfffcU, 0x3fd0U, 1U, 0x3fd8U, 4U)
  CHECK_DIFFERENCE(5U, 1U, 0x4000U, 4U, 0x3ffeU, 1U)
#undef CHECK_DIFFERENCE

#define CHECK_NULL(index, expected, offset, page)                           \
  c166_test_check_u32(base_case + 106U + (index - 1U) * 2U + 1U, expected, \
      tasking_far_is_null_raw_proxy(offset, page));                         \
  c166_test_check_u32(base_case + 106U + (index - 1U) * 2U + 2U, expected, \
      llvm_far_is_null_raw_proxy(offset, page));

  /* Null is the only comparison that observes both stored pointer words. */
  CHECK_NULL(1, 1U, 0U, 0U)
  CHECK_NULL(2, 0U, 0U, 1U)
  CHECK_NULL(3, 0U, 0x4000U, 0U)
  CHECK_NULL(4, 0U, 0x3fd0U, 4U)
#undef CHECK_NULL
}

static void run_far_memory_vector(abi_u16 vector_id, abi_u16 seed,
                                  abi_u32 generated_golden) {
  abi_u16 base_case = 6U +
      ABI_FAR_MEMORY_BOUNDARY_CASES +
      (vector_id - 1U) * ABI_FAR_MEMORY_CASES_PER_VECTOR;
  abi_u16 pointer_base = base_case + 7U;
  abi_u32 expected_hash;
  abi_u32 actual_hash;
  volatile abi_u8 *pointer;

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  expected_hash = tasking_far_apply(seed, &far_page1_arena);
  actual_hash = llvm_far_apply_proxy(seed, &far_page4_arena);
  c166_test_check_u32(base_case + 1U, generated_golden, expected_hash);
  check_pair(base_case + 1U, expected_hash, actual_hash,
             &far_page1_arena, &far_page4_arena);

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  expected_hash = tasking_far_apply(seed, &far_page4_arena);
  actual_hash = llvm_far_apply_proxy(seed, &far_page1_arena);
  check_pair(base_case + 3U, expected_hash, actual_hash,
             &far_page4_arena, &far_page1_arena);

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  expected_hash = tasking_far_apply(seed, &far_page1_arena);
  actual_hash = llvm_far_reverse_apply_proxy(seed, &far_page4_arena);
  check_pair(base_case + 5U, expected_hash, actual_hash,
             &far_page1_arena, &far_page4_arena);

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed ^ 0x55aaU);

#define CHECK_FAR_OFFSET(index, offset)                                      \
  pointer = tasking_far_advance(&far_page1_arena, offset);                   \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 1U, 1UL,           \
      pointer == (volatile abi_u8 *)&far_page1_arena + offset);              \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 2U,                \
      ((volatile abi_u8 *)&far_page1_arena)[offset], *pointer);              \
  pointer = llvm_far_advance_proxy(&far_page4_arena, offset);               \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 3U, 1UL,           \
      pointer == (volatile abi_u8 *)&far_page4_arena + offset);              \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 4U,                \
      ((volatile abi_u8 *)&far_page4_arena)[offset], *pointer);              \
  pointer = llvm_far_reverse_advance_proxy(&far_page4_arena, offset);       \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 5U, 1UL,           \
      pointer == (volatile abi_u8 *)&far_page4_arena + offset);              \
  c166_test_check_u32(pointer_base + (index - 1U) * 6U + 6U,                \
      ((volatile abi_u8 *)&far_page4_arena)[offset], *pointer);

  ABI_FAR_MEMORY_OFFSETS(CHECK_FAR_OFFSET)
#undef CHECK_FAR_OFFSET
}

#define RUN_FAR_MEMORY(id, seed, golden) \
  run_far_memory_vector(id, seed, golden);

void main(void) {
  volatile abi_u8 *base = (volatile abi_u8 *)&far_page1_arena;

  c166_test_begin(273, 0x16600111UL);
  c166_test_check_u32(1, 48UL, sizeof(struct abi_far_arena));
  c166_test_check_u32(2, 4UL, (volatile abi_u8 *)&far_page1_arena.byte0 - base);
  c166_test_check_u32(3, 6UL, (volatile abi_u8 *)&far_page1_arena.word0 - base);
  c166_test_check_u32(4, 12UL, (volatile abi_u8 *)&far_page1_arena.long0 - base);
  c166_test_check_u32(5, 16UL, (volatile abi_u8 *)&far_page1_arena.middle - base);
  c166_test_check_u32(6, 44UL,
                       (volatile abi_u8 *)&far_page1_arena.tail_long - base);
  check_page_boundary(6U, &far_page1_arena, 0x00013fd0UL);
  check_page_boundary(6U + ABI_FAR_MEMORY_CASES_PER_PAGE,
                      &far_page4_arena, 0x00043fd0UL);
  ABI_FAR_MEMORY_VECTORS(RUN_FAR_MEMORY)
  check_far_relations(6U + ABI_FAR_MEMORY_BOUNDARY_CASES +
                      4U * ABI_FAR_MEMORY_CASES_PER_VECTOR);
  c166_test_finish();
  simulator_stop();
}

