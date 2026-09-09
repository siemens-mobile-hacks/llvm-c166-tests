#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long c166_test_entry(unsigned int seed);
extern unsigned long llvm_entry_proxy(unsigned int seed);

static void check(unsigned int id, unsigned int seed, unsigned long expected) {
	c166_test_check_u32(id * 2 - 1, expected, c166_test_entry(seed));
	c166_test_check_u32(id * 2, expected, llvm_entry_proxy(seed));
}

void main(void) {
	c166_test_begin(380, 0x1660017cUL);
	check(1, 0, 0x00010013UL);
	check(2, 1, 0x00020019UL);
	check(3, 0x1234U, 0x00036d4bUL);
	check(4, 0xffffU, 0x0004000dUL);
	c166_test_finish();
	simulator_stop();
}
