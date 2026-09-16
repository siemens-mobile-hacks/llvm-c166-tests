#include "types.h"

typedef abi_u32 (*float_varargs1_fn)(abi_u16, ...);
typedef abi_u32 (*float_varargs2_fn)(abi_u16, abi_u16, ...);
typedef abi_u32 (*float_varargs3_fn)(abi_u16, abi_u16, abi_u16, ...);
typedef abi_u32 (*float_varargs4_fn)(abi_u16, abi_u16, abi_u16, abi_u16, ...);
typedef abi_u32 (*float_varargs5_fn)(abi_u16, abi_u16, abi_u16, abi_u16,
                                    abi_u16, ...);

#define CALL_RAW(target)                                                     \
  switch (shape) {                                                           \
  case 1: return target##1(0x1111U, first, middle, second);                  \
  case 2: return target##2(0x1111U, 0x2222U, first, middle, second);         \
  case 3:                                                                    \
    return target##3(0x1111U, 0x2222U, 0x4444U, first, middle, second);      \
  case 4:                                                                    \
    return target##4(0x1111U, 0x2222U, 0x4444U, 0x8888U, first, middle,     \
                     second);                                                \
  default:                                                                   \
    return target##5(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, first,   \
                     middle, second);                                        \
  }

C166_NOINLINE C166_SECTION(".float_varargs_raw")
abi_u32 float_raw(abi_u16 shape, double first, abi_u16 middle,
                  double second) {
  CALL_RAW(float_varargs)
}

#undef CALL_RAW

C166_NOINLINE C166_SECTION(".float_varargs_raw_indirect")
abi_u32 float_raw_indirect(abi_u16 shape, double first, abi_u16 middle,
                           double second) {
  switch (shape) {
  case 1: {
    volatile float_varargs1_fn function = float_varargs1;
    return function(0x1111U, first, middle, second);
  }
  case 2: {
    volatile float_varargs2_fn function = float_varargs2;
    return function(0x1111U, 0x2222U, first, middle, second);
  }
  case 3: {
    volatile float_varargs3_fn function = float_varargs3;
    return function(0x1111U, 0x2222U, 0x4444U, first, middle, second);
  }
  case 4: {
    volatile float_varargs4_fn function = float_varargs4;
    return function(0x1111U, 0x2222U, 0x4444U, 0x8888U, first, middle,
                    second);
  }
  default: {
    volatile float_varargs5_fn function = float_varargs5;
    return function(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, first,
                    middle, second);
  }
  }
}

#define CALL_PROMOTED(target)                                                \
  switch (shape) {                                                           \
  case 1: return target##1(0x1111U, value, 0x55aaU, 1.0);                   \
  case 2: return target##2(0x1111U, 0x2222U, value, 0x55aaU, 1.0);          \
  case 3:                                                                    \
    return target##3(0x1111U, 0x2222U, 0x4444U, value, 0x55aaU, 1.0);       \
  case 4:                                                                    \
    return target##4(0x1111U, 0x2222U, 0x4444U, 0x8888U, value, 0x55aaU,  \
                     1.0);                                                   \
  default:                                                                   \
    return target##5(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, value,  \
                     0x55aaU, 1.0);                                         \
  }

C166_NOINLINE C166_SECTION(".float_varargs_promote")
abi_u32 float_promote(abi_u16 shape, float value) {
  CALL_PROMOTED(float_varargs)
}

#undef CALL_PROMOTED

C166_NOINLINE C166_SECTION(".float_varargs_promote_indirect")
abi_u32 float_promote_indirect(abi_u16 shape, float value) {
  switch (shape) {
  case 1: {
    volatile float_varargs1_fn function = float_varargs1;
    return function(0x1111U, value, 0x55aaU, 1.0);
  }
  case 2: {
    volatile float_varargs2_fn function = float_varargs2;
    return function(0x1111U, 0x2222U, value, 0x55aaU, 1.0);
  }
  case 3: {
    volatile float_varargs3_fn function = float_varargs3;
    return function(0x1111U, 0x2222U, 0x4444U, value, 0x55aaU, 1.0);
  }
  case 4: {
    volatile float_varargs4_fn function = float_varargs4;
    return function(0x1111U, 0x2222U, 0x4444U, 0x8888U, value, 0x55aaU,
                    1.0);
  }
  default: {
    volatile float_varargs5_fn function = float_varargs5;
    return function(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, value,
                    0x55aaU, 1.0);
  }
  }
}
