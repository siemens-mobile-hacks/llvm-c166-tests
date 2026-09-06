typedef unsigned short u16;
typedef unsigned long u32;

typedef u32 (*size_callback)(u16, u16, u32);

u32 size_indirect_call(size_callback callback, u16 a, u16 b, u32 state) {
  if (callback == 0)
    return state;
  return callback(a, b, state);
}
