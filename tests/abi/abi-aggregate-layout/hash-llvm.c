#include "types.h"

#define DECLARE_PROXY(name, parameters) \
  extern abi_u16 llvm_##name##_proxy parameters

DECLARE_PROXY(layout_chars0, (struct chars3 value, abi_u16 tail));
DECLARE_PROXY(layout_padded1, (abi_u16 head, struct padded value,
                               enum flavor kind, abi_u16 tail));
DECLARE_PROXY(layout_nested2, (abi_u16 head0, abi_u16 head1,
                               struct nested value, abi_u16 tail));
DECLARE_PROXY(layout_union3, (abi_u16 head0, abi_u16 head1, abi_u16 head2,
                              union overlay value, abi_u16 tail));
DECLARE_PROXY(layout_packed0, (ABI_PACKED_REF(packed_value) value,
                               abi_u16 tail));
DECLARE_PROXY(layout_bits3, (abi_u16 head0, abi_u16 head1, abi_u16 head2,
                             struct bit_fields value, abi_u16 tail));
DECLARE_PROXY(layout_packed_bits0,
              (ABI_PACKED_REF(packed_bits_7_10) bits7_10,
               ABI_PACKED_REF(packed_bits_9_8) bits9_8,
               ABI_PACKED_REF(packed_bits_15_2) bits15_2,
               ABI_PACKED_REF(packed_bits_1_16) bits1_16, abi_u16 tail));
DECLARE_PROXY(layout_bit_edges2,
              (abi_u16 head0, abi_u16 head1, struct zero_width_bits zero,
               struct signed_bits signed_value, struct leading_bits leading,
               struct trailing_bits trailing, abi_u16 tail));
DECLARE_PROXY(layout_nested_packed1,
              (abi_u16 head, struct nested_packed_value nested,
               struct packed_array_outer array, struct byte_array_tail bytes,
               abi_u16 tail));

extern abi_u16 llvm_layout_packed_bits0_abi_proxy(
    ABI_PACKED_REF(packed_bits_7_10) *bits7_10,
    ABI_PACKED_REF(packed_bits_9_8) *bits9_8,
    ABI_PACKED_REF(packed_bits_15_2) *bits15_2,
    ABI_PACKED_REF(packed_bits_1_16) *bits1_16, abi_u16 tail);
extern abi_u16 llvm_layout_bit_edges2_abi_proxy(
    abi_u16 head0, abi_u16 head1, struct zero_width_bits *zero,
    struct signed_bits *signed_value, struct leading_bits *leading,
    struct trailing_bits *trailing, abi_u16 tail);

#define ABI_CALL_PACKED_BITS(call, bits7_10, bits9_8, bits15_2, bits1_16, \
                             tail) \
  llvm_layout_packed_bits0_abi_proxy( \
      &(bits7_10), &(bits9_8), &(bits15_2), &(bits1_16), (tail))

#define ABI_CALL_BIT_EDGES(call, head0, head1, zero, signed_value, leading, \
                           trailing, tail) \
  llvm_layout_bit_edges2_abi_proxy( \
      (head0), (head1), &(zero), &(signed_value), &(leading), &(trailing), \
      (tail))

#include "layout-hash.h"

#define ABI_LLVM_CALL(name) llvm_##name##_proxy

abi_u16 hash_llvm_layout(abi_u16 seed, abi_u16 *components) {
  abi_u16 result;
  ABI_HASH_LAYOUT(ABI_LLVM_CALL, ABI_INIT_PACKED_BITS_RAW, seed,
                  components, result);
  return result;
}
