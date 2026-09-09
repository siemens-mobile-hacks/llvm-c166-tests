#ifndef ABI_FP_CALLS_TYPES_H
#define ABI_FP_CALLS_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned char abi_u8;
typedef float (*abi_float_fn)(abi_u16, double, float, abi_u16, abi_u8 *);
typedef double (*abi_double_fn)(abi_u16, float, double, abi_u16, abi_u8 *);

typedef char abi_float_size[sizeof(float) == 4 ? 1 : -1];
typedef char abi_double_size[sizeof(double) == 8 ? 1 : -1];

float tasking_fp_float(abi_u16, double, float, abi_u16, abi_u8 *);
double tasking_fp_double(abi_u16, float, double, abi_u16, abi_u8 *);
float llvm_fp_float_bridge(abi_u16, double, float, abi_u16, abi_u8 *);
double llvm_fp_double_bridge(abi_u16, float, double, abi_u16, abi_u8 *);
void llvm_fp_reverse_bridge(abi_u16, float, double, abi_u8 *);

#endif
