#ifndef ABI_AGGREGATE_LAYOUT_HASH_H
#define ABI_AGGREGATE_LAYOUT_HASH_H

#define ABI_ROTATE5(value)                                                     \
  (abi_u16)(((abi_u16)(value) << 5) | ((abi_u16)(value) >> 11))

#define ABI_COMBINE(result, value, tag)                                        \
  do {                                                                         \
    result = ABI_ROTATE5(result);                                              \
    result ^= (abi_u16)(value);                                                \
    result ^= (abi_u16)(tag);                                                  \
  } while (0)

#define ABI_HASH_LAYOUT(call, seed_value, components, output)                  \
  do {                                                                         \
    struct chars3 chars_value;                                                 \
    struct padded padded_value;                                                \
    struct nested nested_value;                                                \
    union overlay union_value;                                                 \
    ABI_PACKED_REF(packed_value) packed_object;                                \
    struct bit_fields bits_value;                                              \
    ABI_PACKED_REF(packed_bits_7_10) packed_bits7_10;                          \
    ABI_PACKED_REF(packed_bits_9_8) packed_bits9_8;                            \
    ABI_PACKED_REF(packed_bits_15_2) packed_bits15_2;                          \
    union packed_bits_1_16_storage packed_bits1_16;                            \
    struct zero_width_bits zero_bits;                                          \
    struct signed_bits signed_bits_value;                                      \
    struct leading_bits leading_bits_value;                                    \
    struct trailing_bits trailing_bits_value;                                  \
    struct nested_packed_value nested_packed;                                  \
    struct packed_array_outer packed_array;                                    \
    struct byte_array_tail byte_array;                                         \
    enum flavor kind;                                                          \
    abi_u16 abi_seed = (seed_value);                                           \
    abi_u16 abi_result = 0x6d2bU;                                              \
    abi_u16 abi_value;                                                         \
    chars_value.first = (abi_u8)(abi_seed + 0x01U);                            \
    chars_value.second = (abi_u8)(abi_seed + 0x23U);                           \
    chars_value.third = (abi_u8)(abi_seed + 0x45U);                            \
    abi_value = call(layout_chars0)(chars_value, abi_seed ^ 0x1111U);          \
    (components)[0] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0101U);                               \
    padded_value.head = (abi_u8)(abi_seed + 0x12U);                            \
    padded_value.middle = abi_seed ^ 0x2222U;                                  \
    padded_value.tail = (abi_u8)(abi_seed + 0x34U);                            \
    kind = (abi_seed & 1U) ? FLAVOR_NEG : FLAVOR_POS;                          \
    abi_value = call(layout_padded1)(abi_seed + 0x0100U, padded_value, kind,   \
                                     abi_seed ^ 0x3333U);                      \
    (components)[1] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0202U);                               \
    nested_value.prefix = (abi_u8)(abi_seed + 0x21U);                          \
    nested_value.value.tag = (abi_u8)(abi_seed + 0x43U);                       \
    nested_value.value.word = abi_seed ^ 0x4444U;                              \
    nested_value.suffix = (abi_u8)(abi_seed + 0x65U);                          \
    abi_value = call(layout_nested2)(abi_seed + 0x0200U, abi_seed + 0x0300U,   \
                                     nested_value, abi_seed ^ 0x5555U);        \
    (components)[2] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0303U);                               \
    union_value.wide = ((abi_u32)(abi_seed ^ 0x6666U) << 16) |                 \
                       (abi_u32)(abi_u16)(abi_seed + 0x0400U);                 \
    abi_value = call(layout_union3)(abi_seed + 0x0500U, abi_seed + 0x0600U,    \
                                    abi_seed + 0x0700U, union_value,           \
                                    abi_seed ^ 0x7777U);                       \
    (components)[3] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0404U);                               \
    packed_object.head = (abi_u8)(abi_seed + 0x31U);                           \
    packed_object.middle = abi_seed ^ 0x8888U;                                 \
    packed_object.tail = (abi_u8)(abi_seed + 0x53U);                           \
    abi_value = call(layout_packed0)(packed_object, abi_seed ^ 0x9999U);       \
    (components)[4] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0505U);                               \
    bits_value.first = abi_seed & 7U;                                          \
    bits_value.second = (abi_seed >> 3) & 31U;                                 \
    bits_value.third = (abi_seed ^ 0x00a6U) & 0x00ffU;                         \
    bits_value.fourth = (abi_seed >> 8) & 1U;                                  \
    abi_value = call(layout_bits3)(abi_seed + 0x0a00U, abi_seed + 0x0b00U,     \
                                   abi_seed + 0x0c00U, bits_value,             \
                                   abi_seed ^ 0xaaaaU);                        \
    (components)[5] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0606U);                               \
    packed_bits7_10.first = abi_seed & 0x007fU;                                \
    packed_bits7_10.second = (abi_seed ^ 0x0155U) & 0x03ffU;                   \
    packed_bits9_8.first = (abi_seed ^ 0x0123U) & 0x01ffU;                     \
    packed_bits9_8.second = (abi_seed ^ 0x00a5U) & 0x00ffU;                    \
    packed_bits15_2.first = (abi_seed ^ 0x4567U) & 0x7fffU;                    \
    packed_bits15_2.second = (abi_seed >> 6) & 0x0003U;                        \
    packed_bits1_16.value.first = (abi_seed >> 15) & 0x0001U;                  \
    packed_bits1_16.value.second = abi_seed ^ 0xbeefU;                         \
    abi_value = call(layout_packed_bits0)(                                     \
        packed_bits7_10, packed_bits9_8, packed_bits15_2,                      \
        packed_bits1_16.value, abi_seed ^ 0xcafeU);                            \
    (components)[6] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0707U);                               \
    zero_bits.first = abi_seed & 0x0007U;                                      \
    zero_bits.second = (abi_seed >> 3) & 0x001fU;                              \
    signed_bits_value.first = -(abi_s16)((abi_seed & 0x0003U) + 1U);           \
    signed_bits_value.second = -(abi_s16)(((abi_seed >> 2) & 0x00ffU) + 1U);   \
    signed_bits_value.third = (abi_seed >> 10) & 0x000fU;                      \
    leading_bits_value.head = (abi_u8)(abi_seed + 0x17U);                      \
    leading_bits_value.first = (abi_seed ^ 0x0035U) & 0x007fU;                 \
    leading_bits_value.second = (abi_seed ^ 0x0155U) & 0x03ffU;                \
    trailing_bits_value.first = (abi_seed ^ 0x0027U) & 0x007fU;                \
    trailing_bits_value.second = (abi_seed ^ 0x0123U) & 0x01ffU;               \
    trailing_bits_value.tail = (abi_u8)(abi_seed + 0x39U);                     \
    abi_value = call(layout_bit_edges2)(                                       \
        abi_seed + 0x0d00U, abi_seed + 0x0e00U, zero_bits, signed_bits_value,  \
        leading_bits_value, trailing_bits_value, abi_seed ^ 0xdadaU);          \
    (components)[7] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0808U);                               \
    nested_packed.prefix = (abi_u8)(abi_seed + 0x41U);                         \
    nested_packed.inner.byte = (abi_u8)(abi_seed + 0x52U);                     \
    nested_packed.inner.word = abi_seed ^ 0x1357U;                             \
    nested_packed.suffix = (abi_u8)(abi_seed + 0x63U);                         \
    packed_array.prefix = (abi_u8)(abi_seed + 0x74U);                          \
    packed_array.values[0].byte = (abi_u8)(abi_seed + 0x85U);                  \
    packed_array.values[0].word = abi_seed ^ 0x2468U;                          \
    packed_array.values[1].byte = (abi_u8)(abi_seed + 0x96U);                  \
    packed_array.values[1].word = abi_seed ^ 0x369cU;                          \
    packed_array.suffix = (abi_u8)(abi_seed + 0xa7U);                          \
    byte_array.prefix = (abi_u8)(abi_seed + 0xb8U);                            \
    byte_array.bytes[0] = (abi_u8)(abi_seed + 0xc9U);                          \
    byte_array.bytes[1] = (abi_u8)(abi_seed + 0xdaU);                          \
    byte_array.bytes[2] = (abi_u8)(abi_seed + 0xebU);                          \
    byte_array.suffix = (abi_u8)(abi_seed + 0xfcU);                            \
    abi_value = call(layout_nested_packed1)(abi_seed + 0x0f00U, nested_packed, \
                                            packed_array, byte_array,          \
                                            abi_seed ^ 0xebcdU);               \
    (components)[8] = abi_value;                                               \
    ABI_COMBINE(abi_result, abi_value, 0x0909U);                               \
    (output) = abi_result;                                                     \
  } while (0)

#endif
