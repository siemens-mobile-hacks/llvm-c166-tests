#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int case_id, unsigned int flags);

void main(void) {
	unsigned int index, count, initial, cases, id = 0;
	unsigned long expected, actual;

	c166_test_begin(376, 0x166UL);
	for (index = 0; index != 108; ++index) {
		count = (index & 3U) + 1U;
		cases = index >= 68 ? 32 : 1;
		for (initial = 0; initial != cases; ++initial) {
			if (index >= 68) {
				expected = initial;
			} else if (index >= 48) {
				expected = 0x002a0001UL;
			} else if (index < 20 || index >= 28) {
				expected = 0x002a0000UL | (1UL << (count - 1U));
			} else if (index < 24) {
				expected = 0x00200000UL | ((1UL << count) - 1UL);
			} else {
				expected = 1UL;
			}
			c166_test_set_context(0x166UL, index / 4U, count, initial, 0);
			actual = llvm_entry_proxy(index, initial);
			c166_test_check_u32(++id, expected, actual);
			if (actual != expected)
				goto done;
		}
	}
done:
	c166_test_finish();
	simulator_stop();
}
