#include "c166_test.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

typedef void *(*copy_fn)(void *, const void *, c166_test_size_t);
typedef void *(*set_fn)(void *, int, c166_test_size_t);

extern void *memcpy(void *, const void *, c166_test_size_t);
extern void *memmove(void *, const void *, c166_test_size_t);
extern void *memset(void *, int, c166_test_size_t);

static copy_fn volatile call_memcpy = memcpy;
static copy_fn volatile call_memmove = memmove;
static set_fn volatile call_memset = memset;

#if defined(C166_TEST_LLVM) && __C166_MEMORY_MODEL__ != 3 && \
    __C166_MEMORY_MODEL__ != 4
#define PAGE_SECTION(name) __attribute__((section(name)))
#else
#define PAGE_SECTION(name)
#endif

static u8 page_a[64] PAGE_SECTION(".memory.page_a");
static u8 page_b[64] PAGE_SECTION(".memory.page_b");

static void initialize(void) {
  u16 index;

  for (index = 0; index != 64; ++index) {
    page_a[index] = (u8)(index * 37U + 0x29U);
    page_b[index] = (u8)(index * 53U + 0x71U);
  }
}

static u32 digest(const u8 *bytes) {
  u32 hash = 0x1660a55aUL;
  u16 index;

  for (index = 0; index != 64; ++index)
    hash = hash * 257UL + bytes[index] + index;
  return hash;
}

C166_TEST_NOINLINE
u32 c166_test_case(u16 case_id) {
  void *result = page_a;
  void *expected_result = page_a;
  int direct = (case_id & 8U) != 0;

  initialize();
  switch (case_id & 7U) {
  case 0:
    expected_result = page_a + 7;
    result = direct ? memset(page_a + 7, 0x1a5, 23)
                    : call_memset(page_a + 7, 0x1a5, 23);
    break;
  case 1:
    expected_result = page_b + 9;
    result = direct ? memcpy(page_b + 9, page_a + 5, 31)
                    : call_memcpy(page_b + 9, page_a + 5, 31);
    break;
  case 2:
    expected_result = page_a + 4;
    result = direct ? memmove(page_a + 4, page_a + 11, 29)
                    : call_memmove(page_a + 4, page_a + 11, 29);
    break;
  case 3:
    expected_result = page_a + 11;
    result = direct ? memmove(page_a + 11, page_a + 4, 29)
                    : call_memmove(page_a + 11, page_a + 4, 29);
    break;
  case 4:
    expected_result = page_a + 8;
    result = direct ? memmove(page_a + 8, page_a + 8, 32)
                    : call_memmove(page_a + 8, page_a + 8, 32);
    break;
  case 5:
    expected_result = page_b + 3;
    result = direct ? memmove(page_b + 3, page_a + 5, 0)
                    : call_memmove(page_b + 3, page_a + 5, 0);
    break;
  case 6:
    expected_result = page_b;
    result = direct ? memmove(page_b, page_a, 32)
                    : call_memmove(page_b, page_a, 32);
    break;
  default:
    expected_result = page_a;
    result = direct ? memmove(page_a, page_b, 32)
                    : call_memmove(page_a, page_b, 32);
    break;
  }

  return digest(page_a) ^ (digest(page_b) << 1) ^
         (result != expected_result ? 0x80000000UL : 0);
}
