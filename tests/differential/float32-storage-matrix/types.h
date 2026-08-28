#ifndef FLOAT32_STORAGE_TYPES_H
#define FLOAT32_STORAGE_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_float_record {
  abi_u16 tag;
  float first;
  abi_u16 guard;
  float second;
  abi_u32 tail;
};

#endif
