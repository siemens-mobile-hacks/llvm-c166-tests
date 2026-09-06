typedef signed long s32;
typedef unsigned short u16;
typedef unsigned long u32;

u16 size_long_compare(s32 left, s32 right, u32 mask) {
  if (left < right)
    return 1u;
  if ((u32)left > mask)
    return 2u;
  if (((u32)right & mask) == 0UL)
    return 3u;
  return 0u;
}
