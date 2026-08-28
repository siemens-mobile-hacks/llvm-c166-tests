#include "types.h"

extern abi_u16 tasking_layout_packed_bits0_abi_proxy(
    ABI_PACKED_REF(packed_bits_7_10) *bits7_10,
    ABI_PACKED_REF(packed_bits_9_8) *bits9_8,
    ABI_PACKED_REF(packed_bits_15_2) *bits15_2,
    ABI_PACKED_REF(packed_bits_1_16) *bits1_16, abi_u16 tail);
extern abi_u16 tasking_layout_bit_edges2_abi_proxy(
    abi_u16 head0, abi_u16 head1, struct zero_width_bits *zero,
    struct signed_bits *signed_value, struct leading_bits *leading,
    struct trailing_bits *trailing, abi_u16 tail);

#define ABI_CALL_PACKED_BITS(call, bits7_10, bits9_8, bits15_2, bits1_16, \
                             tail) \
  tasking_layout_packed_bits0_abi_proxy( \
      &(bits7_10), &(bits9_8), &(bits15_2), &(bits1_16), (tail))

#define ABI_CALL_BIT_EDGES(call, head0, head1, zero, signed_value, leading, \
                           trailing, tail) \
  tasking_layout_bit_edges2_abi_proxy( \
      (head0), (head1), &(zero), &(signed_value), &(leading), &(trailing), \
      (tail))

#include "layout-hash.h"

#define ABI_TASKING_CALL(name) tasking_##name

abi_u16 hash_tasking_layout(abi_u16 seed, abi_u16 *components) {
  abi_u16 result;
  ABI_HASH_LAYOUT(ABI_TASKING_CALL, ABI_INIT_PACKED_BITS_RAW, seed,
                  components, result);
  return result;
}
