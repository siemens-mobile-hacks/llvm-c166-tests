#include "constants.h"

volatile unsigned int asm_bias = ASM_BIAS;
volatile unsigned int asm_calls;
extern unsigned int c_mix(unsigned int seed, unsigned int bias);

unsigned int asm_stage(unsigned int seed) {
	unsigned int result = c_mix(seed, asm_bias);
	unsigned int index;

	for (index = 0; index < 3; ++index)
		result += seed;
	++asm_calls;
	return result;
}
