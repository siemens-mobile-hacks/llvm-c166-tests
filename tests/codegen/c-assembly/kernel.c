#include "c166_test.h"
#include "constants.h"

extern volatile unsigned int asm_bias;
extern volatile unsigned int asm_calls;
extern unsigned int asm_stage(unsigned int seed);

C166_TEST_NOINLINE unsigned int c_mix(unsigned int seed, unsigned int bias) {
	return seed * 3U + bias;
}

unsigned long c166_test_entry(unsigned int seed) {
	unsigned int before = asm_calls;
	unsigned int result;

	if (asm_bias != ASM_BIAS)
		return 0xdead0001UL;
	result = asm_stage(seed);
	if (asm_calls != (unsigned int)(before + 1U))
		return 0xdead0002UL;
	return ((unsigned long)asm_calls << 16) | result;
}
