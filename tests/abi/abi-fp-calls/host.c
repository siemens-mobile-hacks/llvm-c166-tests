#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"

/* IEEE bits in C166 object order: most significant word first, LE bytes.
 * Include signed zero and nonzero low words; no arithmetic canonicalization. */
static const abi_u8 vectors[4][12] = {
	{0x80, 0x3f, 0, 0, 4, 0xc0, 0, 0, 0, 0, 0, 0},
	{0, 0x80, 0, 0, 0, 0x80, 0, 0, 0, 0, 0, 0},
	{0x91, 0x3f, 0x45, 0x23, 0x23, 0x40, 0x67, 0x45, 0xab, 0x89, 0xef, 0xcd},
	{0x80, 0, 1, 0, 0x10, 0, 1, 0, 2, 0, 3, 0}
};

void main(void) {
	abi_float_fn volatile float_target = llvm_fp_float_bridge;
	abi_double_fn volatile double_target = llvm_fp_double_bridge;
	abi_u8 actual[46];
	abi_u8 expected[46];
	float single, float_result;
	double wide, double_result;
	abi_u16 vector, path, i, case_id = 0;

	c166_test_begin(327, 0x16600147UL);
	for (vector = 0; vector != 4; ++vector) {
		for (i = 0; i != 4; ++i) {
			((abi_u8 *)&single)[i] = vectors[vector][i];
			expected[9 + i] = vectors[vector][i];
			expected[17 + i] = vectors[vector][i];
			expected[21 + i] = vectors[vector][i];
		}
		for (i = 0; i != 8; ++i) {
			((abi_u8 *)&wide)[i] = vectors[vector][4 + i];
			expected[1 + i] = vectors[vector][4 + i];
			expected[25 + i] = vectors[vector][4 + i];
			expected[37 + i] = vectors[vector][4 + i];
		}
		expected[0] = 0xa5;
		expected[45] = 0x5a;
		expected[13] = expected[33] = 0x57;
		expected[14] = expected[34] = 0x13;
		expected[15] = expected[35] = 0xdf;
		expected[16] = expected[36] = 0x9b;

		/* Reference, LLVM direct, LLVM indirect, reverse direct/indirect. */
		for (path = 0; path != 5; ++path) {
			for (i = 0; i != 46; ++i)
				actual[i] = 0xcc;
			actual[0] = 0xa5;
			actual[45] = 0x5a;
			if (path == 0) {
				float_result = tasking_fp_float(0x1357, wide, single, 0x9bdf, actual + 1);
				double_result = tasking_fp_double(0x1357, single, wide, 0x9bdf, actual + 21);
			} else if (path == 1) {
				float_result = llvm_fp_float_bridge(0x1357, wide, single, 0x9bdf, actual + 1);
				double_result = llvm_fp_double_bridge(0x1357, single, wide, 0x9bdf, actual + 21);
			} else if (path == 2) {
				float_result = float_target(0x1357, wide, single, 0x9bdf, actual + 1);
				double_result = double_target(0x1357, single, wide, 0x9bdf, actual + 21);
			} else {
				llvm_fp_reverse_bridge(path - 3, single, wide, actual + 1);
			}
			if (path < 3) {
				for (i = 0; i != 4; ++i)
					actual[17 + i] = ((abi_u8 *)&float_result)[i];
				for (i = 0; i != 8; ++i)
					actual[37 + i] = ((abi_u8 *)&double_result)[i];
			}
			for (i = 0; i != 46; ++i)
				c166_test_check_u32(++case_id, expected[i], actual[i]);
		}
	}
	c166_test_finish();
	simulator_stop();
}
