#include "c166-test-compat.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

typedef void *(*copy_fn)(void *, const void *, u16);
typedef void *(*set_fn)(void *, int, u16);
typedef int (*compare_fn)(const char *, const char *);

extern void *memcpy(void *, const void *, u16);
extern void *memmove(void *, const void *, u16);
extern void *memset(void *, int, u16);
extern int strcmp(const char *, const char *);

static copy_fn volatile call_memcpy = memcpy;
static copy_fn volatile call_memmove = memmove;
static set_fn volatile call_memset = memset;
static compare_fn volatile call_strcmp = strcmp;

#if defined(C166_TEST_LLVM) && __C166_MEMORY_MODEL__ != 3
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
  if (case_id >= 16) {
    const char *a = (const char *)page_a;
    const char *b = (const char *)page_b;
    u16 order = (case_id - 16) % 3;
    int compared;
    page_a[0] = page_b[0] = 'a';
    page_a[1] = 'b';
    page_b[1] = 'c';
    page_a[2] = page_b[2] = 0;
    if (order == 0)
      b = a;
    else if (order == 2) {
      const char *temporary = a;
      a = b;
      b = temporary;
    }
    compared = case_id < 19 ? strcmp(a, b) : call_strcmp(a, b);
    return 1 + (compared > 0) - (compared < 0);
  }
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
