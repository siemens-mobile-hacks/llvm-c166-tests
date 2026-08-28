#ifndef FLOAT64_STORAGE_TYPES_H
#define FLOAT64_STORAGE_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_double_record {
  abi_u16 tag;
  double first;
  abi_u16 guard;
  double second;
  abi_u32 tail;
};

#endif
