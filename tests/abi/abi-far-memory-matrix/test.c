#include "types.h"
#include "vectors.inc"

#if defined(C166_TEST_LLVM)
#define FAR_PAGE1_LOCATION C166_SECTION(".far_page1")
#define FAR_PAGE4_LOCATION C166_SECTION(".far_page4")
#else
#define FAR_PAGE1_LOCATION _at(0x13fd0UL)
#define FAR_PAGE4_LOCATION _at(0x43fd0UL)
#endif

volatile struct abi_far_arena C166_FAR far_page1_arena FAR_PAGE1_LOCATION;
volatile struct abi_far_arena C166_FAR far_page4_arena FAR_PAGE4_LOCATION;

static void fill_arena(abi_far_arena_pointer arena, abi_u16 seed) {
  abi_far_byte_pointer bytes = (abi_far_byte_pointer)arena;
  abi_u16 index;

  for (index = 0; index != 48U; ++index)
    bytes[index] = (abi_u8)(seed + index * 0x25U + 0x5bU);
}

static abi_u32 arena_digest(abi_far_arena_pointer arena) {
  abi_far_byte_pointer bytes = (abi_far_byte_pointer)arena;
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

static void check_pair(abi_u32 expected_hash, abi_u32 actual_hash,
                       abi_far_arena_pointer expected,
                       abi_far_arena_pointer actual) {
  tap_is_u32(actual_hash, expected_hash, "far mutation result");
  tap_is_u32(arena_digest(actual), arena_digest(expected),
             "far mutation bytes");
}

static void check_page_boundary(abi_far_arena_pointer arena,
                                abi_u32 expected_base) {
  abi_far_byte_pointer one_past =
      (abi_far_byte_pointer)arena + sizeof(struct abi_far_arena);
  volatile abi_far_advance_fn indirect_advance = far_advance;
  volatile abi_far_retreat_fn indirect_retreat = far_retreat;

#define CHECK_FORWARD(index, offset)                                       \
  tap_is_u32((abi_u32)far_advance(arena, offset), expected_base + offset,  \
             "far boundary advance");                                     \
  tap_is_u32((abi_u32)far_reverse_advance(arena, offset),                  \
             expected_base + offset, "nested far boundary advance");      \
  tap_is_u32((abi_u32)indirect_advance(arena, offset),                     \
             expected_base + offset, "indirect far boundary advance");

  ABI_FAR_MEMORY_FORWARD_OFFSETS(CHECK_FORWARD)
#undef CHECK_FORWARD

#define CHECK_RETREAT(index, offset)                                       \
  tap_is_u32((abi_u32)far_retreat(one_past, offset),                       \
             expected_base + 48UL - offset, "far boundary retreat");      \
  tap_is_u32((abi_u32)far_reverse_retreat(one_past, offset),               \
             expected_base + 48UL - offset,                               \
             "nested far boundary retreat");                              \
  tap_is_u32((abi_u32)indirect_retreat(one_past, offset),                  \
             expected_base + 48UL - offset,                               \
             "indirect far boundary retreat");

  ABI_FAR_MEMORY_RETREAT_OFFSETS(CHECK_RETREAT)
#undef CHECK_RETREAT
}

static abi_far_byte_pointer far_pointer(abi_u16 offset, abi_u16 page) {
  volatile union {
    abi_far_byte_pointer pointer;
    struct {
      abi_u16 offset;
      abi_u16 page;
    } words;
  } value;

  value.words.offset = offset;
  value.words.page = page;
  return value.pointer;
}

static void check_relation(abi_u16 expected, abi_far_relation_fn function,
                           abi_u16 lhs_offset, abi_u16 lhs_page,
                           abi_u16 rhs_offset, abi_u16 rhs_page) {
  volatile abi_far_relation_fn indirect = function;
  abi_far_byte_pointer lhs = far_pointer(lhs_offset, lhs_page);
  abi_far_byte_pointer rhs = far_pointer(rhs_offset, rhs_page);

  tap_is_u32(function(lhs, rhs), expected, "far pointer relation");
  tap_is_u32(indirect(lhs, rhs), expected, "indirect far pointer relation");
}

static void check_far_relations(void) {
#define CHECK_RELATION_PAIR(index, lhs_offset, lhs_page, rhs_offset, rhs_page) \
  check_relation(lhs_offset == rhs_offset, far_equal, lhs_offset, lhs_page,   \
                 rhs_offset, rhs_page);                                      \
  check_relation(lhs_offset != rhs_offset, far_not_equal, lhs_offset,         \
                 lhs_page, rhs_offset, rhs_page);                             \
  check_relation(lhs_offset < rhs_offset, far_less, lhs_offset, lhs_page,     \
                 rhs_offset, rhs_page);                                      \
  check_relation(lhs_offset <= rhs_offset, far_less_equal, lhs_offset,        \
                 lhs_page, rhs_offset, rhs_page);                             \
  check_relation(lhs_offset > rhs_offset, far_greater, lhs_offset, lhs_page,  \
                 rhs_offset, rhs_page);                                      \
  check_relation(lhs_offset >= rhs_offset, far_greater_equal, lhs_offset,     \
                 lhs_page, rhs_offset, rhs_page);

  CHECK_RELATION_PAIR(1U, 0x0000U, 0U, 0x0000U, 0U)
  CHECK_RELATION_PAIR(2U, 0x0000U, 1U, 0x0000U, 4U)
  CHECK_RELATION_PAIR(3U, 0x0000U, 4U, 0x0001U, 1U)
  CHECK_RELATION_PAIR(4U, 0x0001U, 1U, 0x0000U, 4U)
  CHECK_RELATION_PAIR(5U, 0x3fd0U, 1U, 0x3fd0U, 4U)
  CHECK_RELATION_PAIR(6U, 0x3fd0U, 4U, 0x3fd1U, 1U)
  CHECK_RELATION_PAIR(7U, 0x3fffU, 1U, 0x3ffeU, 4U)
  CHECK_RELATION_PAIR(8U, 0x4000U, 4U, 0x3fffU, 1U)
#undef CHECK_RELATION_PAIR

#define CHECK_DIFFERENCE(index, expected, lhs_offset, lhs_page, rhs_offset, \
                         rhs_page)                                         \
  do {                                                                     \
    volatile abi_far_difference_fn indirect = far_difference;              \
    abi_far_word_pointer lhs =                                             \
        (abi_far_word_pointer)far_pointer(lhs_offset, lhs_page);            \
    abi_far_word_pointer rhs =                                             \
        (abi_far_word_pointer)far_pointer(rhs_offset, rhs_page);            \
    tap_is_u32((abi_u16)far_difference(lhs, rhs), expected,                 \
               "far pointer difference");                                 \
    tap_is_u32((abi_u16)indirect(lhs, rhs), expected,                       \
               "indirect far pointer difference");                        \
  } while (0);

  CHECK_DIFFERENCE(1U, 0U, 0x0000U, 1U, 0x0000U, 4U)
  CHECK_DIFFERENCE(2U, 0xffffU, 0x0000U, 4U, 0x0002U, 1U)
  CHECK_DIFFERENCE(3U, 1U, 0x0002U, 1U, 0x0000U, 4U)
  CHECK_DIFFERENCE(4U, 0xfffcU, 0x3fd0U, 1U, 0x3fd8U, 4U)
  CHECK_DIFFERENCE(5U, 1U, 0x4000U, 4U, 0x3ffeU, 1U)
#undef CHECK_DIFFERENCE

#define CHECK_NULL(index, expected, offset, page)                           \
  do {                                                                     \
    volatile abi_far_null_fn indirect = far_is_null;                       \
    abi_far_byte_pointer pointer = far_pointer(offset, page);               \
    tap_is_u32(far_is_null(pointer), expected, "far pointer null");        \
    tap_is_u32(indirect(pointer), expected, "indirect far pointer null");  \
  } while (0);

  CHECK_NULL(1U, 1U, 0U, 0U)
  CHECK_NULL(2U, 0U, 0U, 1U)
  CHECK_NULL(3U, 0U, 0x4000U, 0U)
  CHECK_NULL(4U, 0U, 0x3fd0U, 4U)
#undef CHECK_NULL
}

static void run_far_memory_vector(abi_u16 seed, abi_u32 expected_hash) {
  abi_u32 page1_hash;
  abi_u32 page4_hash;
  abi_u16 offset;

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  page1_hash = far_apply(seed, &far_page1_arena);
  page4_hash = far_apply(seed, &far_page4_arena);
  tap_is_u32(page1_hash, expected_hash, "far mutation golden");
  check_pair(page1_hash, page4_hash, &far_page1_arena, &far_page4_arena);

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  page1_hash = far_apply(seed, &far_page4_arena);
  page4_hash = far_apply(seed, &far_page1_arena);
  check_pair(page1_hash, page4_hash, &far_page4_arena, &far_page1_arena);

  fill_arena(&far_page1_arena, seed);
  fill_arena(&far_page4_arena, seed);
  page1_hash = far_apply(seed, &far_page1_arena);
  page4_hash = far_reverse_apply(seed, &far_page4_arena);
  check_pair(page1_hash, page4_hash, &far_page1_arena, &far_page4_arena);

#define CHECK_FAR_OFFSET(index, value)                                      \
  offset = value;                                                          \
  tap_ok(far_advance(&far_page1_arena, offset) ==                           \
             (abi_far_byte_pointer)&far_page1_arena + offset,              \
         "far page1 pointer identity");                                    \
  tap_is_u32(*far_advance(&far_page1_arena, offset),                        \
             ((abi_far_byte_pointer)&far_page1_arena)[offset],             \
             "far page1 load");                                           \
  tap_ok(far_advance(&far_page4_arena, offset) ==                           \
             (abi_far_byte_pointer)&far_page4_arena + offset,              \
         "far page4 pointer identity");                                    \
  tap_is_u32(*far_advance(&far_page4_arena, offset),                        \
             ((abi_far_byte_pointer)&far_page4_arena)[offset],             \
             "far page4 load");                                           \
  tap_ok(far_reverse_advance(&far_page4_arena, offset) ==                   \
             (abi_far_byte_pointer)&far_page4_arena + offset,              \
         "nested far pointer identity");                                  \
  tap_is_u32(*far_reverse_advance(&far_page4_arena, offset),                \
             ((abi_far_byte_pointer)&far_page4_arena)[offset],             \
             "nested far load");

  ABI_FAR_MEMORY_OFFSETS(CHECK_FAR_OFFSET)
#undef CHECK_FAR_OFFSET
}

#define RUN_FAR_MEMORY(id, seed, expected)                                 \
  run_far_memory_vector(seed, expected);

void main(void) {
  abi_far_byte_pointer base = (abi_far_byte_pointer)&far_page1_arena;

  tap_plan(406U);
  tap_is_u32(sizeof(struct abi_far_arena), 48UL, "far arena size");
  tap_is_u32((abi_far_byte_pointer)&far_page1_arena.byte0 - base, 4UL,
             "byte0 offset");
  tap_is_u32((abi_far_byte_pointer)&far_page1_arena.word0 - base, 6UL,
             "word0 offset");
  tap_is_u32((abi_far_byte_pointer)&far_page1_arena.long0 - base, 12UL,
             "long0 offset");
  tap_is_u32((abi_far_byte_pointer)&far_page1_arena.middle - base, 16UL,
             "middle offset");
  tap_is_u32((abi_far_byte_pointer)&far_page1_arena.tail_long - base, 44UL,
             "tail offset");
  check_page_boundary(&far_page1_arena, 0x00013fd0UL);
  check_page_boundary(&far_page4_arena, 0x00043fd0UL);
  ABI_FAR_MEMORY_VECTORS(RUN_FAR_MEMORY)
  check_far_relations();
}
