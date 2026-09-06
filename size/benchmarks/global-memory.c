typedef unsigned short u16;
typedef unsigned long u32;

extern volatile u16 size_global_word;
extern volatile u32 size_global_long;

u32 size_global_memory(u16 value) {
  u32 current = size_global_long;
  size_global_word = (u16)(size_global_word + value);
  size_global_long = current + size_global_word;
  return current;
}
