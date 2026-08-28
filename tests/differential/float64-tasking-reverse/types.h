#ifndef FLOAT64_TASKING_REVERSE_TYPES_H
#define FLOAT64_TASKING_REVERSE_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned char abi_u8;

typedef union {
  double value;
  abi_u16 words[4];
} abi_double_words;

typedef struct {
  abi_u8 bytes[8];
} abi_block8;

#endif
