#ifndef ABI_VARARGS_AGGREGATE_TYPES_H
#define ABI_VARARGS_AGGREGATE_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#ifdef __clang__
#define ABI_PACKED_REF(name) struct name
#else
#define ABI_PACKED_REF(name) _packed struct name
#endif

struct pair2 {
  abi_u16 first;
  abi_u16 second;
};

struct chars3 {
  abi_u8 first;
  abi_u8 second;
  abi_u8 third;
};

#ifdef __clang__
struct __attribute__((packed)) packed3 {
#else
_packed struct packed3 {
#endif
  abi_u8 first;
  abi_u16 second;
};

typedef char assert_pair2_size[sizeof(struct pair2) == 4 ? 1 : -1];
typedef char assert_chars3_size[sizeof(struct chars3) == 4 ? 1 : -1];
typedef char assert_packed3_size[
    sizeof(ABI_PACKED_REF(packed3)) == 3 ? 1 : -1];

abi_u16 tasking_varargs_aggregate(abi_u16 fixed, ...);
abi_u16 llvm_varargs_aggregate(abi_u16 fixed, ...);

abi_u16 tasking_varargs_aggregate_abi_proxy(
    struct pair2 *pair_value, struct chars3 *chars_value,
    ABI_PACKED_REF(packed3) *packed_value, abi_u16 tail);
abi_u16 llvm_varargs_aggregate_abi_proxy(
    struct pair2 *pair_value, struct chars3 *chars_value,
    ABI_PACKED_REF(packed3) *packed_value, abi_u16 tail);
abi_u32 llvm_varargs_aggregate_reverse_proxy(abi_u16 seed);

#endif
