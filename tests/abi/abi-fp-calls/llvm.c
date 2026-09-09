#include "types.h"

#define FLOAT_FUNCTION llvm_fp_float
#define DOUBLE_FUNCTION llvm_fp_double
#define FLOAT_ATTRIBUTE __attribute__((noinline, section(".llvm_fp_float")))
#define DOUBLE_ATTRIBUTE __attribute__((noinline, section(".llvm_fp_double")))
#include "callee.inc"

__attribute__((noinline, section(".llvm_fp_reverse")))
void llvm_fp_reverse(abi_u16 indirect, float single, double wide, abi_u8 *out) {
	abi_float_fn volatile float_target = tasking_fp_float;
	abi_double_fn volatile double_target = tasking_fp_double;
	float float_result;
	double double_result;
	abi_u16 i;

	if (indirect) {
		float_result = float_target(0x1357, wide, single, 0x9bdf, out);
		double_result = double_target(0x1357, single, wide, 0x9bdf, out + 20);
	} else {
		float_result = tasking_fp_float(0x1357, wide, single, 0x9bdf, out);
		double_result = tasking_fp_double(0x1357, single, wide, 0x9bdf, out + 20);
	}
	for (i = 0; i != 4; ++i)
		out[16 + i] = ((abi_u8 *)&float_result)[i];
	for (i = 0; i != 8; ++i)
		out[36 + i] = ((abi_u8 *)&double_result)[i];
}
