#ifndef FLOAT_VARARGS_MATRIX_TYPES_H
#define FLOAT_VARARGS_MATRIX_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

union abi_double_words {
  double value;
  abi_u16 words[4];
};

union abi_float_words {
  float value;
  abi_u16 words[2];
};

extern volatile union abi_double_words vararg_captured[2];
extern volatile abi_u16 vararg_observed[2];

abi_u32 float_varargs1(abi_u16 fixed0, ...);
abi_u32 float_varargs2(abi_u16 fixed0, abi_u16 fixed1, ...);
abi_u32 float_varargs3(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...);
abi_u32 float_varargs4(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                       abi_u16 fixed3, ...);
abi_u32 float_varargs5(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2,
                       abi_u16 fixed3, abi_u16 fixed4, ...);

abi_u32 float_raw(abi_u16 shape, double first, abi_u16 middle,
                  double second);
abi_u32 float_raw_indirect(abi_u16 shape, double first, abi_u16 middle,
                           double second);
abi_u32 float_promote(abi_u16 shape, float value);
abi_u32 float_promote_indirect(abi_u16 shape, float value);

#endif
