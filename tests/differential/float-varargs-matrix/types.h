#ifndef FLOAT_VARARGS_MATRIX_TYPES_H
#define FLOAT_VARARGS_MATRIX_TYPES_H

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

abi_u32 tasking_float_varargs1(abi_u16, ...);
abi_u32 tasking_float_varargs2(abi_u16, abi_u16, ...);
abi_u32 tasking_float_varargs3(abi_u16, abi_u16, abi_u16, ...);
abi_u32 tasking_float_varargs4(abi_u16, abi_u16, abi_u16, abi_u16, ...);
abi_u32 tasking_float_varargs5(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, ...);
abi_u32 llvm_float_varargs_proxy1(abi_u16, ...);
abi_u32 llvm_float_varargs_proxy2(abi_u16, abi_u16, ...);
abi_u32 llvm_float_varargs_proxy3(abi_u16, abi_u16, abi_u16, ...);
abi_u32 llvm_float_varargs_proxy4(abi_u16, abi_u16, abi_u16, abi_u16, ...);
abi_u32 llvm_float_varargs_proxy5(abi_u16, abi_u16, abi_u16, abi_u16, abi_u16, ...);
abi_u32 tasking_float_raw(abi_u16, double, abi_u16, double);
abi_u32 tasking_float_promote(abi_u16, float);
abi_u32 llvm_float_raw_proxy(abi_u16, double, abi_u16, double);
abi_u32 llvm_float_promote_proxy(abi_u16, float);

#endif
