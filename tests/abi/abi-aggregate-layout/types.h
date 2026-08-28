#ifndef ABI_AGGREGATE_LAYOUT_TYPES_H
#define ABI_AGGREGATE_LAYOUT_TYPES_H

typedef unsigned char abi_u8;
typedef signed int abi_s16;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#ifdef __clang__
#define ABI_PACKED_REF(name) struct name
#else
#define ABI_PACKED_REF(name) _packed struct name
#endif

struct chars3 {
  abi_u8 first;
  abi_u8 second;
  abi_u8 third;
};

struct padded {
  abi_u8 head;
  abi_u16 middle;
  abi_u8 tail;
};

struct inner {
  abi_u8 tag;
  abi_u16 word;
};

struct nested {
  abi_u8 prefix;
  struct inner value;
  abi_u8 suffix;
};

union overlay {
  abi_u8 bytes[3];
  abi_u16 words[2];
  abi_u32 wide;
};

#ifdef __clang__
struct __attribute__((packed)) packed_value {
#else
_packed struct packed_value {
#endif
  abi_u8 head;
  abi_u16 middle;
  abi_u8 tail;
};

struct bit_fields {
  unsigned int first : 3;
  unsigned int second : 5;
  unsigned int third : 8;
  unsigned int fourth : 1;
};

struct zero_width_bits {
  unsigned int first : 3;
  unsigned int : 0;
  unsigned int second : 5;
};

struct signed_bits {
  signed int first : 3;
  signed int second : 9;
  unsigned int third : 4;
};

struct leading_bits {
  abi_u8 head;
  unsigned int first : 7;
  unsigned int second : 10;
};

struct trailing_bits {
  unsigned int first : 7;
  unsigned int second : 9;
  abi_u8 tail;
};

#ifdef __clang__
struct __attribute__((packed)) packed_bits_7_10 {
#else
_packed struct packed_bits_7_10 {
#endif
  unsigned int first : 7;
  unsigned int second : 10;
};

#ifdef __clang__
struct __attribute__((packed)) packed_bits_9_8 {
#else
_packed struct packed_bits_9_8 {
#endif
  unsigned int first : 9;
  unsigned int second : 8;
};

#ifdef __clang__
struct __attribute__((packed)) packed_bits_15_2 {
#else
_packed struct packed_bits_15_2 {
#endif
  unsigned int first : 15;
  unsigned int second : 2;
};

#ifdef __clang__
struct __attribute__((packed)) packed_bits_1_16 {
#else
_packed struct packed_bits_1_16 {
#endif
  unsigned int first : 1;
  unsigned int second : 16;
};

union packed_bits_1_16_storage {
  ABI_PACKED_REF(packed_bits_1_16) value;
  abi_u8 bytes[3];
};

#ifdef __clang__
struct __attribute__((packed)) packed_inner_value {
#else
_packed struct packed_inner_value {
#endif
  abi_u8 byte;
  abi_u16 word;
};

struct nested_packed_value {
  abi_u8 prefix;
  ABI_PACKED_REF(packed_inner_value) inner;
  abi_u8 suffix;
};

#ifdef __clang__
struct __attribute__((packed)) packed_nested_value {
#else
_packed struct packed_nested_value {
#endif
  abi_u8 prefix;
  ABI_PACKED_REF(packed_inner_value) inner;
  abi_u8 suffix;
};

struct packed_array_outer {
  abi_u8 prefix;
  ABI_PACKED_REF(packed_inner_value) values[2];
  abi_u8 suffix;
};

struct byte_array_tail {
  abi_u8 prefix;
  abi_u8 bytes[3];
  abi_u8 suffix;
};

enum flavor {
  FLAVOR_NEG = -1,
  FLAVOR_ZERO = 0,
  FLAVOR_POS = 0x1234
};

typedef char assert_chars3_size[sizeof(struct chars3) == 4 ? 1 : -1];
typedef char assert_padded_size[sizeof(struct padded) == 6 ? 1 : -1];
typedef char assert_inner_size[sizeof(struct inner) == 4 ? 1 : -1];
typedef char assert_nested_size[sizeof(struct nested) == 8 ? 1 : -1];
typedef char assert_union_size[sizeof(union overlay) == 4 ? 1 : -1];
typedef char assert_packed_size[
    sizeof(ABI_PACKED_REF(packed_value)) == 4 ? 1 : -1];
typedef char assert_bit_fields_size[
    sizeof(struct bit_fields) == 4 ? 1 : -1];
typedef char assert_zero_width_bits_size[
    sizeof(struct zero_width_bits) == 4 ? 1 : -1];
typedef char assert_signed_bits_size[
    sizeof(struct signed_bits) == 2 ? 1 : -1];
typedef char assert_leading_bits_size[
    sizeof(struct leading_bits) == 4 ? 1 : -1];
typedef char assert_trailing_bits_size[
    sizeof(struct trailing_bits) == 4 ? 1 : -1];
typedef char assert_packed_bits_7_10_size[
    sizeof(ABI_PACKED_REF(packed_bits_7_10)) == 3 ? 1 : -1];
typedef char assert_packed_bits_9_8_size[
    sizeof(ABI_PACKED_REF(packed_bits_9_8)) == 3 ? 1 : -1];
typedef char assert_packed_bits_15_2_size[
    sizeof(ABI_PACKED_REF(packed_bits_15_2)) == 3 ? 1 : -1];
typedef char assert_packed_bits_1_16_size[
    sizeof(ABI_PACKED_REF(packed_bits_1_16)) == 3 ? 1 : -1];
typedef char assert_packed_inner_size[
    sizeof(ABI_PACKED_REF(packed_inner_value)) == 3 ? 1 : -1];
typedef char assert_nested_packed_size[
    sizeof(struct nested_packed_value) == 6 ? 1 : -1];
typedef char assert_packed_nested_size[
    sizeof(ABI_PACKED_REF(packed_nested_value)) == 5 ? 1 : -1];
typedef char assert_packed_array_outer_size[
    sizeof(struct packed_array_outer) == 10 ? 1 : -1];
typedef char assert_byte_array_tail_size[
    sizeof(struct byte_array_tail) == 6 ? 1 : -1];
typedef char assert_enum_size[sizeof(enum flavor) == 2 ? 1 : -1];

#define DECLARE_LAYOUT(prefix) \
  abi_u16 prefix##_layout_chars0(struct chars3 value, abi_u16 tail); \
  abi_u16 prefix##_layout_padded1(abi_u16 head, struct padded value, \
                                  enum flavor kind, abi_u16 tail); \
  abi_u16 prefix##_layout_nested2(abi_u16 head0, abi_u16 head1, \
                                  struct nested value, abi_u16 tail); \
  abi_u16 prefix##_layout_union3(abi_u16 head0, abi_u16 head1, \
                                 abi_u16 head2, union overlay value, \
                                 abi_u16 tail); \
  abi_u16 prefix##_layout_packed0(ABI_PACKED_REF(packed_value) value, \
                                  abi_u16 tail); \
  abi_u16 prefix##_layout_bits3(abi_u16 head0, abi_u16 head1, \
                                abi_u16 head2, struct bit_fields value, \
                                abi_u16 tail); \
  abi_u16 prefix##_layout_packed_bits0( \
      ABI_PACKED_REF(packed_bits_7_10) bits7_10, \
      ABI_PACKED_REF(packed_bits_9_8) bits9_8, \
      ABI_PACKED_REF(packed_bits_15_2) bits15_2, \
      ABI_PACKED_REF(packed_bits_1_16) bits1_16, abi_u16 tail); \
  abi_u16 prefix##_layout_bit_edges2( \
      abi_u16 head0, abi_u16 head1, struct zero_width_bits zero, \
      struct signed_bits signed_value, struct leading_bits leading, \
      struct trailing_bits trailing, abi_u16 tail); \
  abi_u16 prefix##_layout_nested_packed1( \
      abi_u16 head, struct nested_packed_value nested, \
      struct packed_array_outer array, struct byte_array_tail bytes, \
      abi_u16 tail)

DECLARE_LAYOUT(tasking);
DECLARE_LAYOUT(llvm);

#endif
