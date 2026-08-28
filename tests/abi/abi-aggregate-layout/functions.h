#ifndef ABI_AGGREGATE_LAYOUT_FUNCTIONS_H
#define ABI_AGGREGATE_LAYOUT_FUNCTIONS_H

#define DEFINE_LAYOUT_FUNCTIONS(fn_prefix, attr0, attr1, attr2, attr3, attr4, \
                                attr5, attr6) \
  attr0 abi_u16 fn_prefix##_layout_chars0(struct chars3 value, \
                                           abi_u16 tail) { \
    return (abi_u16)(value.first + value.second * 3U + value.third * 5U + \
                     tail * 7U); \
  } \
  attr1 abi_u16 fn_prefix##_layout_padded1( \
      abi_u16 head, struct padded value, enum flavor kind, abi_u16 tail) { \
    return (abi_u16)(head * 3U + value.head * 5U + value.middle * 7U + \
                     value.tail * 11U + (abi_u16)kind * 13U + tail * 17U); \
  } \
  attr2 abi_u16 fn_prefix##_layout_nested2( \
      abi_u16 head0, abi_u16 head1, struct nested value, abi_u16 tail) { \
    return (abi_u16)(head0 * 3U + head1 * 5U + value.prefix * 7U + \
                     value.value.tag * 11U + value.value.word * 13U + \
                     value.suffix * 17U + tail * 19U); \
  } \
  attr3 abi_u16 fn_prefix##_layout_union3( \
      abi_u16 head0, abi_u16 head1, abi_u16 head2, union overlay value, \
      abi_u16 tail) { \
    return (abi_u16)(head0 * 3U + head1 * 5U + head2 * 7U + \
                     (abi_u16)value.wide * 11U + \
                     (abi_u16)(value.wide >> 16) * 13U + tail * 17U); \
  } \
  attr4 abi_u16 fn_prefix##_layout_packed0( \
      ABI_PACKED_REF(packed_value) value, abi_u16 tail) { \
    return (abi_u16)(value.head * 3U + value.middle * 5U + \
                     value.tail * 7U + tail * 11U); \
  } \
  attr5 abi_u16 fn_prefix##_layout_bits3( \
      abi_u16 head0, abi_u16 head1, abi_u16 head2, \
      struct bit_fields value, abi_u16 tail) { \
    return (abi_u16)(head0 * 3U + head1 * 5U + head2 * 7U + \
                     value.first * 11U + value.second * 13U + \
                     value.third * 17U + value.fourth * 19U + tail * 23U); \
  } \
  attr6 abi_u16 fn_prefix##_layout_packed_bits0( \
      ABI_PACKED_REF(packed_bits_7_10) bits7_10, \
      ABI_PACKED_REF(packed_bits_9_8) bits9_8, \
      ABI_PACKED_REF(packed_bits_15_2) bits15_2, \
      ABI_PACKED_REF(packed_bits_1_16) bits1_16, abi_u16 tail) { \
    return (abi_u16)(bits7_10.first * 3U + bits7_10.second * 5U + \
                     bits9_8.first * 7U + bits9_8.second * 11U + \
                     bits15_2.first * 13U + bits15_2.second * 17U + \
                     bits1_16.first * 19U + bits1_16.second * 23U + \
                     tail * 29U); \
  }

#define DEFINE_LAYOUT_BIT_EDGES_FUNCTION(fn_prefix, attr) \
  attr abi_u16 fn_prefix##_layout_bit_edges2( \
      abi_u16 head0, abi_u16 head1, struct zero_width_bits zero, \
      struct signed_bits signed_value, struct leading_bits leading, \
      struct trailing_bits trailing, abi_u16 tail) { \
    return (abi_u16)(head0 * 3U + head1 * 5U + zero.first * 7U + \
                     zero.second * 11U + \
                     (abi_u16)signed_value.first * 13U + \
                     (abi_u16)signed_value.second * 17U + \
                     signed_value.third * 19U + leading.head * 23U + \
                     leading.first * 29U + leading.second * 31U + \
                     trailing.first * 37U + trailing.second * 41U + \
                     trailing.tail * 43U + tail * 47U); \
  }

#define DEFINE_LAYOUT_NESTED_PACKED_FUNCTION(fn_prefix, attr) \
  attr abi_u16 fn_prefix##_layout_nested_packed1( \
      abi_u16 head, struct nested_packed_value nested, \
      struct packed_array_outer array, struct byte_array_tail bytes, \
      abi_u16 tail) { \
    return (abi_u16)(head * 3U + nested.prefix * 5U + \
                     nested.inner.byte * 7U + nested.inner.word * 11U + \
                     nested.suffix * 13U + array.prefix * 17U + \
                     array.values[0].byte * 19U + \
                     array.values[0].word * 23U + \
                     array.values[1].byte * 29U + \
                     array.values[1].word * 31U + array.suffix * 37U + \
                     bytes.prefix * 41U + bytes.bytes[0] * 43U + \
                     bytes.bytes[1] * 47U + bytes.bytes[2] * 53U + \
                     bytes.suffix * 59U + tail * 61U); \
  }

#endif
