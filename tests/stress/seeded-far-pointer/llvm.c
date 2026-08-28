#include "types.h"

__attribute__((noinline))
abi_u32 llvm_seeded_far_eval(abi_u16 operation,
                             volatile struct abi_seeded_far_arena *arena,
                             abi_u16 index, abi_u16 other, abi_u32 value) {
  abi_u16 lhs;
  abi_u16 rhs;
  abi_u16 step;
  abi_s16 difference;
  volatile abi_u8 *byte_lhs;
  volatile abi_u8 *byte_rhs;
  volatile abi_u16 *word_lhs;
  volatile abi_u16 *word_rhs;
  volatile struct abi_seeded_far_record *record;
  volatile struct abi_seeded_far_record *neighbor;

  switch (operation) {
  case 0:
    return arena->bytes[index % ABI_SEEDED_FAR_BYTE_COUNT];
  case 1:
    lhs = index % ABI_SEEDED_FAR_BYTE_COUNT;
    arena->bytes[lhs] = (abi_u8)value;
    return arena->bytes[lhs];
  case 2:
    lhs = index % ABI_SEEDED_FAR_BYTE_COUNT;
    arena->bytes[lhs] ^= (abi_u8)(value >> 8);
    return arena->bytes[lhs];
  case 3:
    return arena->words[index % ABI_SEEDED_FAR_WORD_COUNT];
  case 4:
    lhs = index % ABI_SEEDED_FAR_WORD_COUNT;
    arena->words[lhs] = (abi_u16)value;
    return arena->words[lhs];
  case 5:
    lhs = index % ABI_SEEDED_FAR_WORD_COUNT;
    arena->words[lhs] = (abi_u16)(arena->words[lhs] + (abi_u16)value);
    return arena->words[lhs];
  case 6:
    return arena->longs[index % ABI_SEEDED_FAR_LONG_COUNT];
  case 7:
    lhs = index % ABI_SEEDED_FAR_LONG_COUNT;
    arena->longs[lhs] = value;
    return arena->longs[lhs];
  case 8:
    lhs = index % ABI_SEEDED_FAR_LONG_COUNT;
    arena->longs[lhs] ^= value;
    return arena->longs[lhs];
  case 9:
    record = &arena->records[index % ABI_SEEDED_FAR_RECORD_COUNT];
    return record->wide ^ ((abi_u32)record->word << 16) ^
           ((abi_u32)record->second << 8) ^ record->first;
  case 10:
    record = &arena->records[index % ABI_SEEDED_FAR_RECORD_COUNT];
    record->first = (abi_u8)(record->first + (abi_u8)value);
    record->second ^= (abi_u8)(value >> 8);
    record->word = (abi_u16)(record->word + (abi_u16)(value >> 16));
    record->wide ^= value;
    return record->wide ^ ((abi_u32)record->word << 16) ^
           ((abi_u32)record->second << 8) ^ record->first;
  case 11:
    lhs = index % ABI_SEEDED_FAR_RECORD_COUNT;
    rhs = (abi_u16)((lhs + 1U) % ABI_SEEDED_FAR_RECORD_COUNT);
    record = &arena->records[lhs];
    neighbor = &arena->records[rhs];
    return record->wide + neighbor->word +
           ((abi_u32)neighbor->first << 24);
  case 12:
    lhs = index % ABI_SEEDED_FAR_BYTE_COUNT;
    rhs = other % ABI_SEEDED_FAR_BYTE_COUNT;
    byte_lhs = &arena->bytes[lhs];
    byte_rhs = &arena->bytes[rhs];
    difference = byte_lhs - byte_rhs;
    return ((abi_u32)(abi_u16)difference << 16) |
           ((abi_u16)*byte_lhs << 8) |
           (byte_lhs == byte_rhs) | ((abi_u16)(byte_lhs < byte_rhs) << 1) |
           ((abi_u16)(byte_lhs > byte_rhs) << 2);
  case 13:
    lhs = index % ABI_SEEDED_FAR_BYTE_COUNT;
    step = other % (ABI_SEEDED_FAR_BYTE_COUNT - lhs);
    byte_lhs = &arena->bytes[lhs];
    byte_rhs = byte_lhs + step;
    return ((abi_u32)step << 16) | *byte_rhs;
  case 14:
    lhs = index % ABI_SEEDED_FAR_WORD_COUNT;
    rhs = other % ABI_SEEDED_FAR_WORD_COUNT;
    word_lhs = &arena->words[lhs];
    word_rhs = &arena->words[rhs];
    difference = word_lhs - word_rhs;
    return ((abi_u32)(abi_u16)difference << 16) | *word_lhs;
  default:
    return arena->longs[index % ABI_SEEDED_FAR_LONG_COUNT] ^
           arena->records[other % ABI_SEEDED_FAR_RECORD_COUNT].wide ^
           ((abi_u32)arena->words[other % ABI_SEEDED_FAR_WORD_COUNT] << 16) ^
           arena->bytes[index % ABI_SEEDED_FAR_BYTE_COUNT];
  }
}
