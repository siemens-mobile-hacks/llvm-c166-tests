#ifndef ABI_FP_CALLS_TYPES_H
#define ABI_FP_CALLS_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned char abi_u8;
typedef float (*abi_float_fn)(abi_u16, double, float, abi_u16, abi_u8 *);
typedef double (*abi_double_fn)(abi_u16, float, double, abi_u16, abi_u8 *);

typedef char abi_float_size[sizeof(float) == 4 ? 1 : -1];
typedef char abi_double_size[sizeof(double) == 8 ? 1 : -1];

float abi_fp_float(abi_u16, double, float, abi_u16, abi_u8 *);
double abi_fp_double(abi_u16, float, double, abi_u16, abi_u8 *);
void abi_fp_reverse(abi_u16, float, double, abi_u8 *);

#endif
