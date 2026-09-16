#include "kernel.h"

test_u16 alloca_callback(const test_u8 *bytes, test_u16 count, test_u16 seed,
                         test_u16 first, test_u16 second) {
  test_u16 index;
  test_u16 state = (test_u16)(seed ^ first ^ (second << 1) ^ count);

  for (index = 0; index != count; ++index) {
    test_u8 value = bytes[index];
    if (value != dynamic_byte(seed, index))
      return (test_u16)(0xbad0U ^ index);
    state = fold_byte(state, value);
  }
  return state;
}
