#ifndef SEEDED_INTEGER_EXPRESSIONS_INC
#define SEEDED_INTEGER_EXPRESSIONS_INC

#define ABI_SEEDED_INTEGER_EXPRESSIONS(X) \
  X(0, u8_add, (abi_u8)(ua8 + ub8)) \
  X(1, u8_sub, (abi_u8)(ua8 - ub8)) \
  X(2, u8_xor, (abi_u8)(ua8 ^ ub8)) \
  X(3, u8_not, (abi_u8)~ua8) \
  X(4, u8_shl, (abi_u8)((abi_u16)ua8 << (ub8 & 7U))) \
  X(5, u8_shr, (abi_u8)(ua8 >> (ub8 & 7U))) \
  X(6, u8_lt, ua8 < ub8) \
  X(7, u8_select, ua8 != 0U ? ub8 : (abi_u8)0xa5U) \
  X(8, s8_add, (abi_s16)sa8 + (abi_s16)sb8) \
  X(9, s8_sub, (abi_s16)sa8 - (abi_s16)sb8) \
  X(10, s8_mul, (abi_s16)sa8 * (abi_s16)sb8) \
  X(11, s8_neg, -(abi_s16)sa8) \
  X(12, s8_lt, sa8 < sb8) \
  X(13, s8_ge, sa8 >= sb8) \
  X(14, s8_min, sa8 < sb8 ? (abi_s16)sa8 : (abi_s16)sb8) \
  X(15, s8_max, sa8 > sb8 ? (abi_s16)sa8 : (abi_s16)sb8) \
  X(16, u16_add, (abi_u16)(ua16 + ub16)) \
  X(17, u16_sub, (abi_u16)(ua16 - ub16)) \
  X(18, u16_mul, (abi_u16)(ua16 * ub16)) \
  X(19, u16_and, ua16 & ub16) \
  X(20, u16_or, ua16 | ub16) \
  X(21, u16_shl, (abi_u16)((abi_u32)ua16 << (ub16 & 15U))) \
  X(22, u16_shr, ua16 >> (ub16 & 15U)) \
  X(23, u16_div, ua16 / (abi_u16)(ub16 | 1U)) \
  X(24, u16_mod, ua16 % (abi_u16)(ub16 | 1U)) \
  X(25, u16_lt, ua16 < ub16) \
  X(26, s16_add_wide, (abi_s32)sa16 + (abi_s32)sb16) \
  X(27, s16_sub_wide, (abi_s32)sa16 - (abi_s32)sb16) \
  X(28, s16_neg_wide, -(abi_s32)sa16) \
  X(29, s16_div, sb16 == 0 ? 0UL : (sa16 == (abi_s16)0x8000U && sb16 == (abi_s16)-1 ? 0x8000UL : (abi_u32)(sa16 / sb16))) \
  X(30, s16_mod, sb16 == 0 ? 0UL : (sa16 == (abi_s16)0x8000U && sb16 == (abi_s16)-1 ? 0UL : (abi_u32)(sa16 % sb16))) \
  X(31, s16_lt, sa16 < sb16) \
  X(32, s16_ge, sa16 >= sb16) \
  X(33, s16_min, sa16 < sb16 ? (abi_s32)sa16 : (abi_s32)sb16) \
  X(34, s16_max, sa16 > sb16 ? (abi_s32)sa16 : (abi_s32)sb16) \
  X(35, u32_add, a + b) \
  X(36, u32_sub, a - b) \
  X(37, u32_neg, 0UL - a) \
  X(38, u32_and, a & b) \
  X(39, u32_or, a | b) \
  X(40, u32_xor, a ^ b) \
  X(41, u32_not, ~a) \
  X(42, u32_shl, a << (ub16 & 31U)) \
  X(43, u32_shr, a >> (ub16 & 31U)) \
  X(44, u32_lt, a < b) \
  X(45, u32_select, a >= b ? a : b) \
  X(46, s32_lt, sa32 < sb32) \
  X(47, s32_le, sa32 <= sb32) \
  X(48, s32_gt, sa32 > sb32) \
  X(49, s32_ge, sa32 >= sb32) \
  X(50, s32_min, sa32 < sb32 ? a : b) \
  X(51, s32_max, sa32 > sb32 ? a : b) \
  X(52, s32_s16_add, (abi_s32)sa16 + (abi_s32)sb16) \
  X(53, s32_s16_abs, sa16 < 0 ? -(abi_s32)sa16 : (abi_s32)sa16)

#endif
