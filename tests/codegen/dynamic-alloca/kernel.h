#ifndef C166_DYNAMIC_ALLOCA_KERNEL_H
#define C166_DYNAMIC_ALLOCA_KERNEL_H

typedef unsigned char test_u8;
typedef unsigned int test_u16;

static test_u8 dynamic_byte(test_u16 seed, test_u16 index) {
  return (test_u8)(seed + index * 13U + (index >> 1));
}

static test_u16 fold_byte(test_u16 state, test_u8 value) {
  return (test_u16)(((state << 5) | (state >> 11)) ^ value ^ 0x136dU);
}

static test_u16 expected_callback(test_u16 count, test_u16 seed,
                                  test_u16 first, test_u16 second) {
  test_u16 index;
  test_u16 state = (test_u16)(seed ^ first ^ (second << 1) ^ count);

  for (index = 0; index != count; ++index)
    state = fold_byte(state, dynamic_byte(seed, index));
  return state;
}

#endif
