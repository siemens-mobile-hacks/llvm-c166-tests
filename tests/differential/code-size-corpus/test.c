#include "c166_test.h"

typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef struct size_pair {
  u16 low;
  u16 high;
} size_pair;

typedef struct size_triple {
  u16 first;
  u16 second;
  u16 third;
} size_triple;

typedef struct size_flags {
  unsigned mode : 3;
  unsigned ready : 1;
  unsigned count : 6;
  unsigned error : 1;
  unsigned reserved : 5;
} size_flags;

typedef u32 (*size_callback)(u16, u16, u32);

u32 size_aggregate_arguments(size_pair, u16, u16, u16);
size_triple size_aggregate_return(u16, u16, u16);
u16 size_bitfields(size_flags *, u16);
u16 size_high_bit_is_clear(u16);
u16 size_middle_bit_is_clear(u16);
s16 size_signed_byte_load(const s8 *, s16);
u16 size_unsigned_byte_load(const u8 *, u16);
u16 size_byte_indexed_loop(const u8 *, u8 *, u16);
u16 size_callee_saved(u16, u16, u16, u16);
u16 size_conditional_branches(s16, u16, u16);
double size_double_arithmetic(double, double);
u16 size_double_comparison(double, double);
double size_signed_to_double(signed long);
double size_unsigned_to_double(unsigned long);
u16 size_far_pointers(const char *, u16);
float size_float_arithmetic(float, float, u16);
u16 size_frame_address_pressure(void);
u32 size_global_memory(u16);
u16 size_immediates(u16);
u16 size_indexed_loop(const u16 *, u16 *, u16);
u32 size_indirect_call(size_callback, u16, u16, u32);
u32 size_long_arithmetic(u32, u32);
u16 size_long_compare(signed long, signed long, u32);
u32 size_long_indexed_loop(const u32 *, u16);
u32 size_long_shift_constant(u32);
u32 size_long_shift_variable(u32, u16);
u32 size_multiply_divide(u16, u16, u16);
u16 size_outgoing_stack(u16, u16, u16, u16);
s16 size_path_redirect(char *, u16, u16, u16 *);
u16 size_readonly_data(void);
u16 size_stack_address(const char *, u16);
u16 size_stack_arguments(u16, u16, u16, u16, u16, u16);
u16 size_switch_dense(u16, u16);
u16 size_switch_sparse(u16, u16);
u32 size_tail_call(u16, u32);
u32 size_varargs(unsigned int, ...);

volatile u16 size_global_word;
volatile u32 size_global_long;

u16 size_sink1(u16 value) { return (u16)(value ^ 0x1357u); }

u16 size_sink4(u16 a, u16 b, u16 c, u16 d) {
  return (u16)(a + (u16)(b * 3u) + (u16)(c * 5u) + (u16)(d * 7u));
}

u16 size_sink6(u16 a, u16 b, u16 c, u16 d, u16 e, u16 f) {
  return (u16)(a ^ (u16)(b << 1) ^ (u16)(c << 2) ^ (u16)(d << 3) ^
               (u16)(e << 4) ^ (u16)(f << 5));
}

u32 size_consume_pair(size_pair pair, u16 a, u16 b, u16 c) {
  return ((u32)(pair.low ^ a) << 16) | (u16)(pair.high + b + c);
}

u16 size_consume(const char *text) {
  u16 hash = 0x166u;
  while (*text != '\0') {
    hash = (u16)((hash << 5) ^ (u8)*text);
    ++text;
  }
  return hash;
}

void size_observe4(char *a, char *b, char *c, char *d) {
  *a = 1;
  *b = 3;
  *c = 5;
  *d = 7;
}

void size_barrier(void) {}

u16 size_consume4(char *a, char *b, char *c, char *d) {
  return (u16)((u8)*a + ((u16)(u8)*b << 4) + ((u16)(u8)*c << 8) +
               ((u16)(u8)*d << 12));
}

s16 size_compare(const char *left, const char *right) {
  while (*left != '\0' && *left == *right) {
    ++left;
    ++right;
  }
  return (s16)((u8)*left - (u8)*right);
}

char *size_copy(char *target, const char *source) {
  char *result = target;
  do {
    *target++ = *source;
  } while (*source++ != '\0');
  return result;
}

char *size_find_last(const char *text, int character) {
  char *result = 0;
  while (*text != '\0') {
    if ((u8)*text == (u8)character)
      result = (char *)text;
    ++text;
  }
  return result;
}

s16 size_open(char *path, u16 flags, u16 mode, u16 *error) {
  u16 hash;
  if (path == 0) {
    *error = 1u;
    return -1;
  }
  hash = (u16)(flags ^ (u16)(mode << 3));
  while (*path != '\0') {
    hash = (u16)((hash << 3) ^ (u8)*path);
    ++path;
  }
  *error = 0u;
  return (s16)hash;
}

u32 size_tail_target(u16 selector, u32 state) {
  return state ^ ((u32)selector << 16) ^ selector;
}

double size_tail_double_target(double left, double right) {
  return left + right;
}

static u32 size_callback_target(u16 a, u16 b, u32 state) {
  return state + ((u32)a << 16) + b;
}

C166_TEST_NOINLINE u32 c166_test_case(u16 case_id) {
#ifdef SIZE_CORPUS_FLOATING_ONLY
#ifndef SIZE_CORPUS_CASE_OFFSET
#define SIZE_CORPUS_CASE_OFFSET 0
#endif
  case_id = (u16)(case_id + SIZE_CORPUS_CASE_OFFSET);
  if (case_id < 2u)
    case_id = (u16)(case_id + 15u);
  else
    case_id = (u16)(case_id + 17u);
#else
  if (case_id >= 17u)
    case_id = (u16)(case_id + 6u);
  else if (case_id >= 15u)
    case_id = (u16)(case_id + 2u);
#endif
  switch (case_id) {
#ifndef SIZE_CORPUS_FLOATING_ONLY
  case 0: {
    size_pair pair = {0x1234u, 0x5678u};
    return size_aggregate_arguments(pair, 3u, 5u, 7u);
  }
  case 1: {
    size_pair pair = {0xffffu, 0x8000u};
    return size_aggregate_arguments(pair, 0x7fffu, 0x00ffu, 0xff00u);
  }
  case 2: {
    size_triple value = size_aggregate_return(3u, 5u, 7u);
    return ((u32)value.first << 16) ^ ((u32)value.second << 8) ^ value.third;
  }
  case 3: {
    size_triple value = size_aggregate_return(0xffffu, 1u, 0x8000u);
    return ((u32)value.first << 16) ^ ((u32)value.second << 8) ^ value.third;
  }
  case 4: {
    size_flags flags = {3u, 0u, 17u, 0u, 0u};
    u16 value = size_bitfields(&flags, 29u);
    return ((u32)value << 16) | ((u16)flags.count << 2) | flags.ready;
  }
  case 5: {
    size_flags flags = {7u, 1u, 63u, 1u, 0u};
    u16 value = size_bitfields(&flags, 3u);
    return ((u32)value << 16) | ((u16)flags.count << 2) | flags.ready;
  }
  case 6: {
    s8 value = -100;
    return (u16)size_signed_byte_load(&value, 123);
  }
  case 7: {
    s8 value = 100;
    return (u16)size_signed_byte_load(&value, -23);
  }
  case 8: {
    u8 value = 0xe5u;
    return size_unsigned_byte_load(&value, 0x3fu);
  }
  case 9: {
    u8 source[5] = {1u, 3u, 5u, 7u, 9u};
    u8 destination[5];
    u16 value = size_byte_indexed_loop(source, destination, 5u);
    return ((u32)value << 16) | ((u16)destination[0] << 8) | destination[4];
  }
  case 10: {
    u8 source[1] = {0x55u};
    u8 destination[1] = {0xaau};
    return ((u32)size_byte_indexed_loop(source, destination, 0u) << 16) |
           destination[0];
  }
  case 11:
    return size_callee_saved(1u, 2u, 3u, 4u);
  case 12:
    return size_conditional_branches(-123, 10u, 100u);
  case 13:
    return size_conditional_branches(5, 10u, 100u);
  case 14:
    return size_conditional_branches(123, 10u, 100u);
#else
  case 15:
    return (u32)size_double_arithmetic(3.0, 4.0);
  case 16:
    return (u32)(signed long)size_double_arithmetic(-5.0, 7.0);
#endif
#ifndef SIZE_CORPUS_FLOATING_ONLY
  case 17:
    return size_far_pointers("abcdef", 2u);
  case 18:
    return size_far_pointers(0, 5u);
#else
  case 19:
    return (u32)size_float_arithmetic(3.0f, 2.0f, 0u);
  case 20:
    return (u32)size_float_arithmetic(3.0f, 2.0f, 1u);
  case 21:
    return (u32)size_float_arithmetic(3.0f, 2.0f, 2u);
  case 22:
    return (u32)size_float_arithmetic(6.0f, 2.0f, 3u);
  case 23:
    return size_double_comparison(-0.0, 0.0);
  case 24:
    return size_double_comparison(-5.0, 7.0);
  case 25:
    return (u32)(signed long)size_signed_to_double(-123456789L);
  case 26:
    return (u32)size_unsigned_to_double(0xf1234567UL);
#endif
#ifndef SIZE_CORPUS_FLOATING_ONLY
  case 23:
    return size_frame_address_pressure();
  case 24:
    size_global_word = 0x1234u;
    size_global_long = 0x56789abcUL;
    return size_global_memory(0x1111u) ^ size_global_long;
  case 25:
    return size_immediates(0x4321u);
  case 26: {
    u16 source[4] = {1u, 10u, 100u, 1000u};
    u16 destination[4];
    u16 value = size_indexed_loop(source, destination, 4u);
    return ((u32)value << 16) | destination[3];
  }
  case 27:
    return size_indirect_call(size_callback_target, 3u, 5u, 0x12345678UL);
  case 28:
    return size_indirect_call(0, 3u, 5u, 0x12345678UL);
  case 29:
    return size_long_arithmetic(0x12345678UL, 0xa5a55a5aUL);
  case 30:
    return size_long_compare(-5L, 7L, 0x0000ffffUL);
  case 31:
    return size_long_compare(0x12345L, 0x10000L, 0x0000ffffUL);
  case 32: {
    u32 source[4] = {1UL, 0x10000UL, 0x12345678UL, 0xffffffffUL};
    return size_long_indexed_loop(source, 4u);
  }
  case 33:
    return size_long_shift_constant(0x12345678UL);
  case 34:
    return size_long_shift_variable(0x12345678UL, 13u);
  case 35:
    return size_multiply_divide(0x1234u, 0x0055u, 7u);
  case 36:
    return size_outgoing_stack(1u, 2u, 3u, 4u);
  case 37: {
    char path[32] = "dir\\source.bin";
    u16 error;
    return ((u32)(u16)size_path_redirect(path, 3u, 5u, &error) << 16) | error;
  }
  case 38: {
    char path[32] = "dir\\other.bin";
    u16 error;
    return ((u32)(u16)size_path_redirect(path, 3u, 5u, &error) << 16) | error;
  }
  case 39:
    return size_readonly_data();
  case 40:
    return size_stack_address("abcdef", 3u);
  case 41:
    return size_stack_arguments(1u, 2u, 3u, 4u, 5u, 6u);
  case 42:
    return size_switch_dense(0u, 0x1234u);
  case 43:
    return size_switch_dense(1u, 0x1234u);
  case 44:
    return size_switch_dense(2u, 0x1234u);
  case 45:
    return size_switch_dense(3u, 0x1234u);
  case 46:
    return size_switch_dense(4u, 0x1234u);
  case 47:
    return size_switch_dense(5u, 0x1234u);
  case 48:
    return size_switch_dense(6u, 0x1234u);
  case 49:
    return size_switch_dense(7u, 0x1234u);
  case 50:
    return size_switch_dense(8u, 0x1234u);
  case 51:
    return size_switch_sparse(1u, 0x1234u);
  case 52:
    return size_switch_sparse(17u, 0x1234u);
  case 53:
    return size_switch_sparse(257u, 0x1234u);
  case 54:
    return size_switch_sparse(4096u, 0x1234u);
  case 55:
    return size_switch_sparse(0x7fffu, 0x1234u);
  case 56:
    return size_switch_sparse(2u, 0x1234u);
  case 57:
    return size_tail_call(0x1234u, 0x56789abcUL);
  case 58:
    return size_varargs(0u);
  case 59:
    return size_varargs(4u, (unsigned int)1u, (unsigned int)3u,
                        (unsigned int)5u, (unsigned int)7u);
  case 60:
    size_global_word = 0xffffu;
    size_global_long = 1UL;
    return size_global_memory(2u) ^ size_global_long;
  case 61: {
    u32 source[1] = {0x12345678UL};
    return size_long_indexed_loop(source, 0u);
  }
  case 62: {
    size_pair pair = {0u, 0u};
    return size_aggregate_arguments(pair, 0u, 0u, 0u);
  }
  case 63:
    return size_high_bit_is_clear(0x8000u);
  case 64:
    return size_middle_bit_is_clear(0u);
  default:
    return size_conditional_branches((s16)0x8000u, 10u, 100u);
#else
  default:
    return 0;
#endif
  }
}
