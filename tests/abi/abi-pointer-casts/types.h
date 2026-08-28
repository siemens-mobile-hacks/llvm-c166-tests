#ifndef ABI_POINTER_CASTS_TYPES_H
#define ABI_POINTER_CASTS_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef void (*abi_callback)(void);

struct abi_pointer_cast_input {
  abi_u32 linear;
  volatile abi_u16 *data;
  abi_callback function;
};

struct abi_pointer_cast_output {
  abi_u32 data_to_long;
  volatile abi_u16 *long_to_data;
  abi_u32 data_roundtrip;
  abi_u32 function_to_long;
  abi_callback long_to_function;
};

#endif
