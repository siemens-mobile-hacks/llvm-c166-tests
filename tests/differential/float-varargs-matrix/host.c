#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"
#include "vectors.inc"

extern volatile abi_u16 vararg_captured_words[8];
extern volatile abi_u16 vararg_observed[2];

static abi_u16 fixed_mix(abi_u16 shape) {
	switch (shape) {
		case 1:
			return 0x1111U;
		case 2:
			return 0x3333U;
		case 3:
			return 0x7777U;
		case 4:
			return 0xffffU;
		default:
			return 0x5432U;
	}
}

static void check_capture(abi_u16 base, abi_u16 shape, abi_u32 result, abi_u16 first3, abi_u16 first2, abi_u16 first1,
						  abi_u16 first0, abi_u16 middle, abi_u16 second3, abi_u16 second2, abi_u16 second1,
						  abi_u16 second0) {
	abi_u16 mix = fixed_mix(shape);
	c166_test_check_u32(base, 0xa5a50000UL | mix, result);
	c166_test_check_u32(base + 1U, first3, vararg_captured_words[0]);
	c166_test_check_u32(base + 2U, first2, vararg_captured_words[1]);
	c166_test_check_u32(base + 3U, first1, vararg_captured_words[2]);
	c166_test_check_u32(base + 4U, first0, vararg_captured_words[3]);
	c166_test_check_u32(base + 5U, middle, vararg_observed[0]);
	c166_test_check_u32(base + 6U, second3, vararg_captured_words[4]);
	c166_test_check_u32(base + 7U, second2, vararg_captured_words[5]);
	c166_test_check_u32(base + 8U, second1, vararg_captured_words[6]);
	c166_test_check_u32(base + 9U, second0, vararg_captured_words[7]);
	c166_test_check_u32(base + 10U, mix, vararg_observed[1]);
}

static void run_raw_vector(abi_u16 vector_id, abi_u16 first3, abi_u16 first2, abi_u16 first1, abi_u16 first0,
						   abi_u16 middle, abi_u16 second3, abi_u16 second2, abi_u16 second1, abi_u16 second0) {
	union abi_double_words first;
	union abi_double_words second;
	abi_u16 shape;
	first.words[0] = first3;
	first.words[1] = first2;
	first.words[2] = first1;
	first.words[3] = first0;
	second.words[0] = second3;
	second.words[1] = second2;
	second.words[2] = second1;
	second.words[3] = second0;
	for (shape = 1; shape <= 5; ++shape) {
		abi_u16 path;
		abi_u16 base = (abi_u16)(0x7000U + ((vector_id - 1U) * 5U + shape - 1U) * 11U);
		for (path = 0; path != 2; ++path) {
			abi_u32 result = path ? llvm_float_raw_proxy(shape, first.value, middle, second.value)
								  : tasking_float_raw(shape, first.value, middle, second.value);
			check_capture(base + path * 0x1000U, shape, result, first3, first2, first1, first0, middle, second3,
						  second2, second1, second0);
		}
	}
}

static void run_promotion_vector(abi_u16 vector_id, abi_u16 float1, abi_u16 float0, abi_u16 double3, abi_u16 double2,
								 abi_u16 double1, abi_u16 double0) {
	union abi_float_words input;
	abi_u16 shape;
	input.words[0] = float1;
	input.words[1] = float0;
	for (shape = 1; shape <= 5; ++shape) {
		abi_u16 path;
		abi_u16 base = (abi_u16)(0x7000U + 275U + ((vector_id - 1U) * 5U + shape - 1U) * 11U);
		for (path = 0; path != 2; ++path) {
			abi_u32 result =
				path ? llvm_float_promote_proxy(shape, input.value) : tasking_float_promote(shape, input.value);
			check_capture(base + path * 0x1000U, shape, result, double3, double2, double1, double0, 0x55aaU, 0x3ff0U,
						  0x0000U, 0x0000U, 0x0000U);
		}
	}
}

#define RUN_RAW(id, a3, a2, a1, a0, m, b3, b2, b1, b0) run_raw_vector(id, a3, a2, a1, a0, m, b3, b2, b1, b0);
#define RUN_PROMOTION(id, f1, f0, d3, d2, d1, d0) run_promotion_vector(id, f1, f0, d3, d2, d1, d0);

void main(void) {
	c166_test_begin(300, 0x1660012cUL);
	ABI_FLOAT_VARARGS_RAW_VECTORS(RUN_RAW)
	ABI_FLOAT_VARARGS_PROMOTION_VECTORS(RUN_PROMOTION)
	c166_test_finish();
	simulator_stop();
}
