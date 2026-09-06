typedef unsigned char u8;
typedef unsigned short u16;

u16 size_byte_compare(u8 left, u8 right) { return left < right; }

u16 size_byte_compare_immediate(u8 value) { return value < 3u; }

u16 size_byte_compare_full_immediate(u8 value) { return value < 200u; }

void size_byte_increment(u8 *value) { ++*value; }

void size_byte_decrement(u8 *value) { --*value; }

void size_byte_xor(u8 *value) { *value ^= 1u; }

void size_byte_and(u8 *value) { *value &= 6u; }

void size_byte_or(u8 *value) { *value |= 7u; }

void size_byte_add_full_immediate(u8 *value) { *value = (u8)(*value + 200u); }

void size_byte_subtract_full_immediate(u8 *value) {
  *value = (u8)(*value - 129u);
}

void size_byte_xor_full_immediate(u8 *value) { *value ^= 0xc3u; }

void size_byte_and_full_immediate(u8 *value) { *value &= 0xf0u; }

void size_byte_or_full_immediate(u8 *value) { *value |= 0x81u; }

void size_byte_negate(u8 *value) { *value = (u8)(0u - *value); }

void size_byte_complement(u8 *value) { *value = (u8) ~*value; }

void size_byte_add_value(u8 *value, const u8 *operand) {
  *value = (u8)(*value + *operand);
}

void size_byte_subtract_value(u8 *value, const u8 *operand) {
  *value = (u8)(*value - *operand);
}

void size_byte_xor_value(u8 *value, const u8 *operand) {
  *value = (u8)(*value ^ *operand);
}

void size_byte_and_value(u8 *value, const u8 *operand) {
  *value = (u8)(*value & *operand);
}

void size_byte_or_value(u8 *value, const u8 *operand) {
  *value = (u8)(*value | *operand);
}
