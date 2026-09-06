typedef signed short s16;
typedef unsigned short u16;

u16 size_conditional_branches(s16 value, u16 lower, u16 upper) {
  u16 magnitude;
  if (value < 0)
    magnitude = (u16)(0u - (u16)value);
  else
    magnitude = (u16)value;

  if (magnitude < lower)
    return lower;
  if (magnitude > upper)
    return upper;
  return magnitude;
}
