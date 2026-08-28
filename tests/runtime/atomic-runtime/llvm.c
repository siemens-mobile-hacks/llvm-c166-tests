typedef unsigned char u8;
typedef unsigned int u16;
typedef signed long s32;
typedef unsigned long u32;

#ifndef C166_ATOMIC_TEST_PART
#define C166_ATOMIC_TEST_PART 0
#endif

#define C166_ATOMIC_TEST_INTEGER                                             \
  (C166_ATOMIC_TEST_PART == 0 || C166_ATOMIC_TEST_PART == 1)
#define C166_ATOMIC_TEST_FLOATING                                            \
  (C166_ATOMIC_TEST_PART == 0 || C166_ATOMIC_TEST_PART == 2)
#define C166_ATOMIC_TEST_AGGREGATE                                           \
  (C166_ATOMIC_TEST_PART == 0 || C166_ATOMIC_TEST_PART == 3)
#define C166_ATOMIC_TEST_LOCKING                                             \
  (C166_ATOMIC_TEST_PART == 0 || C166_ATOMIC_TEST_PART == 4)

typedef struct __attribute__((packed)) {
  u16 word;
  u8 byte;
} triple;

typedef union {
  float value;
  u16 words[2];
} float_bits;

typedef union {
  double value;
  u16 words[4];
} double_bits;

static _Atomic(u8) atomic_u8;
static _Atomic(u16) atomic_u16;
static _Atomic(s32) atomic_s32;
static _Atomic(u32) atomic_u32;
static _Atomic(float) atomic_float;
static _Atomic(double) atomic_double;
static _Atomic(triple) atomic_triple;

#define CHECK(condition, code)                                                 \
  do {                                                                         \
    if (!(condition))                                                          \
      return (code);                                                           \
  } while (0)

static int triple_equal(triple left, triple right) {
  return left.word == right.word && left.byte == right.byte;
}

static int float_equal(float left, float right) {
  float_bits left_bits = {.value = left};
  float_bits right_bits = {.value = right};
  return left_bits.words[0] == right_bits.words[0] &&
         left_bits.words[1] == right_bits.words[1];
}

static int double_equal(double left, double right) {
  double_bits left_bits = {.value = left};
  double_bits right_bits = {.value = right};
  return left_bits.words[0] == right_bits.words[0] &&
         left_bits.words[1] == right_bits.words[1] &&
         left_bits.words[2] == right_bits.words[2] &&
         left_bits.words[3] == right_bits.words[3];
}

static u16 read_psw(void) {
  u16 value;
  __asm__ volatile("mov %0, psw" : "=r"(value));
  return value;
}

static void disable_interrupts(void) {
  __asm__ volatile("bclr psw.11" ::: "cc", "memory");
}

static void enable_interrupts(void) {
  __asm__ volatile("bset psw.11" ::: "cc", "memory");
}

unsigned int llvm_entry(void) {
#if C166_ATOMIC_TEST_INTEGER
  u8 expected_u8;
  u8 old_u8;
  u16 old_u16;
  s32 old_s32;
  u32 old_u32;
#endif
#if C166_ATOMIC_TEST_FLOATING
  float old_float;
  double old_double;
#endif
#if C166_ATOMIC_TEST_AGGREGATE
  triple first = {0x1234, 0x56};
  triple second = {0xabcd, 0xef};
  triple third = {0x789a, 0xbc};
  triple expected_triple;
  triple old_triple;
#endif
#if C166_ATOMIC_TEST_LOCKING
  u16 psw_before;
  u16 psw_after;
#endif

#if C166_ATOMIC_TEST_INTEGER
  __c11_atomic_init(&atomic_u8, 0x12);
  CHECK(__c11_atomic_load(&atomic_u8, __ATOMIC_RELAXED) == 0x12, 1);
  __c11_atomic_store(&atomic_u8, 0x34, __ATOMIC_RELEASE);
  old_u8 = __c11_atomic_exchange(&atomic_u8, 0x56, __ATOMIC_ACQ_REL);
  CHECK(old_u8 == 0x34, 2);
  CHECK(__c11_atomic_load(&atomic_u8, __ATOMIC_ACQUIRE) == 0x56, 3);

  expected_u8 = 0x56;
  CHECK(__c11_atomic_compare_exchange_strong(
            &atomic_u8, &expected_u8, 0x78, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE),
        4);
  CHECK(expected_u8 == 0x56, 5);
  CHECK(__c11_atomic_load(&atomic_u8, __ATOMIC_RELAXED) == 0x78, 6);
  expected_u8 = 0x77;
  CHECK(!__c11_atomic_compare_exchange_weak(&atomic_u8, &expected_u8, 0x99,
                                            __ATOMIC_ACQ_REL, __ATOMIC_RELAXED),
        7);
  CHECK(expected_u8 == 0x78, 8);
  CHECK(__c11_atomic_load(&atomic_u8, __ATOMIC_RELAXED) == 0x78, 9);

  __c11_atomic_init(&atomic_u16, 0x1234);
  old_u16 = __c11_atomic_fetch_add(&atomic_u16, 0x0102, __ATOMIC_SEQ_CST);
  CHECK(old_u16 == 0x1234, 10);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0x1336, 11);
  old_u16 = __c11_atomic_fetch_sub(&atomic_u16, 0x0036, __ATOMIC_RELAXED);
  CHECK(old_u16 == 0x1336, 12);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0x1300, 13);

  __c11_atomic_store(&atomic_u16, 0x55aa, __ATOMIC_RELAXED);
  old_u16 = __c11_atomic_fetch_and(&atomic_u16, 0x0ff0, __ATOMIC_RELAXED);
  CHECK(old_u16 == 0x55aa, 14);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0x05a0, 15);
  old_u16 = __c11_atomic_fetch_or(&atomic_u16, 0xa005, __ATOMIC_RELAXED);
  CHECK(old_u16 == 0x05a0, 16);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0xa5a5, 17);
  old_u16 = __c11_atomic_fetch_xor(&atomic_u16, 0xffff, __ATOMIC_RELAXED);
  CHECK(old_u16 == 0xa5a5, 18);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0x5a5a, 19);
  old_u16 = __c11_atomic_fetch_nand(&atomic_u16, 0x0ff0, __ATOMIC_RELAXED);
  CHECK(old_u16 == 0x5a5a, 20);
  CHECK(__c11_atomic_load(&atomic_u16, __ATOMIC_RELAXED) == 0xf5af, 21);

  __c11_atomic_init(&atomic_s32, -10L);
  old_s32 = __c11_atomic_fetch_max(&atomic_s32, -3L, __ATOMIC_RELAXED);
  CHECK(old_s32 == -10L, 22);
  CHECK(__c11_atomic_load(&atomic_s32, __ATOMIC_RELAXED) == -3L, 23);
  old_s32 = __c11_atomic_fetch_max(&atomic_s32, -20L, __ATOMIC_RELAXED);
  CHECK(old_s32 == -3L, 24);
  CHECK(__c11_atomic_load(&atomic_s32, __ATOMIC_RELAXED) == -3L, 25);
  old_s32 = __c11_atomic_fetch_min(&atomic_s32, -20L, __ATOMIC_RELAXED);
  CHECK(old_s32 == -3L, 26);
  CHECK(__c11_atomic_load(&atomic_s32, __ATOMIC_RELAXED) == -20L, 27);

  __c11_atomic_init(&atomic_u32, 10UL);
  old_u32 = __c11_atomic_fetch_max(&atomic_u32, 20UL, __ATOMIC_RELAXED);
  CHECK(old_u32 == 10UL, 28);
  CHECK(__c11_atomic_load(&atomic_u32, __ATOMIC_RELAXED) == 20UL, 29);
  old_u32 = __c11_atomic_fetch_min(&atomic_u32, 7UL, __ATOMIC_RELAXED);
  CHECK(old_u32 == 20UL, 30);
  CHECK(__c11_atomic_load(&atomic_u32, __ATOMIC_RELAXED) == 7UL, 31);
#endif

#if C166_ATOMIC_TEST_FLOATING
  __c11_atomic_init(&atomic_float, 1.5f);
  CHECK(float_equal(__c11_atomic_load(&atomic_float, __ATOMIC_RELAXED), 1.5f),
        36);
  __c11_atomic_store(&atomic_float, -2.25f, __ATOMIC_RELEASE);
  old_float = __c11_atomic_exchange(&atomic_float, 3.5f, __ATOMIC_ACQ_REL);
  CHECK(float_equal(old_float, -2.25f), 37);
  CHECK(float_equal(__c11_atomic_load(&atomic_float, __ATOMIC_ACQUIRE), 3.5f),
        38);

  __c11_atomic_init(&atomic_double, 6.25);
  CHECK(double_equal(__c11_atomic_load(&atomic_double, __ATOMIC_RELAXED), 6.25),
        39);
  __c11_atomic_store(&atomic_double, -8.5, __ATOMIC_RELEASE);
  old_double = __c11_atomic_exchange(&atomic_double, 0.125, __ATOMIC_ACQ_REL);
  CHECK(double_equal(old_double, -8.5), 40);
  CHECK(double_equal(__c11_atomic_load(&atomic_double, __ATOMIC_ACQUIRE),
                     0.125),
        41);
#endif

#if C166_ATOMIC_TEST_AGGREGATE
  __c11_atomic_init(&atomic_triple, first);
  CHECK(
      triple_equal(__c11_atomic_load(&atomic_triple, __ATOMIC_RELAXED), first),
      42);
  __c11_atomic_store(&atomic_triple, second, __ATOMIC_RELEASE);
  old_triple = __c11_atomic_exchange(&atomic_triple, third, __ATOMIC_ACQ_REL);
  CHECK(triple_equal(old_triple, second), 43);
  CHECK(
      triple_equal(__c11_atomic_load(&atomic_triple, __ATOMIC_ACQUIRE), third),
      44);
  expected_triple = third;
  CHECK(__c11_atomic_compare_exchange_strong(&atomic_triple, &expected_triple,
                                             first, __ATOMIC_SEQ_CST,
                                             __ATOMIC_ACQUIRE),
        45);
  CHECK(triple_equal(expected_triple, third), 46);
  expected_triple = second;
  CHECK(!__c11_atomic_compare_exchange_strong(&atomic_triple, &expected_triple,
                                              third, __ATOMIC_SEQ_CST,
                                              __ATOMIC_RELAXED),
        47);
  CHECK(triple_equal(expected_triple, first), 48);
#endif

#if C166_ATOMIC_TEST_INTEGER
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_u8)), 49);
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_u16)), 50);
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_u32)), 51);
#endif
#if C166_ATOMIC_TEST_FLOATING
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_float)), 58);
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_double)), 52);
#endif
#if C166_ATOMIC_TEST_AGGREGATE
  CHECK(!__c11_atomic_is_lock_free(sizeof(atomic_triple)), 53);
#endif
#if C166_ATOMIC_TEST_LOCKING
#if C166_ATOMIC_TEST_PART == 4
  __c11_atomic_init(&atomic_u16, 0x1234);
#endif
  __c11_atomic_thread_fence(__ATOMIC_SEQ_CST);

  disable_interrupts();
  psw_before = read_psw();
  (void)__c11_atomic_fetch_add(&atomic_u16, 1, __ATOMIC_SEQ_CST);
  psw_after = read_psw();
  CHECK(((psw_before ^ psw_after) & 0x0800) == 0, 54);
  CHECK((psw_after & 0x0800) == 0, 55);

  enable_interrupts();
  psw_before = read_psw();
  (void)__c11_atomic_fetch_add(&atomic_u16, 1, __ATOMIC_SEQ_CST);
  psw_after = read_psw();
  disable_interrupts();
  CHECK((psw_before & 0x0800) != 0, 56);
  CHECK((psw_after & 0x0800) != 0, 57);
#endif

  return 0;
}
