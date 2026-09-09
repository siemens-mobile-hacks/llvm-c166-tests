#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int case_id);

void main(void) {
	unsigned int case_id;
	unsigned long expected, actual;

	c166_test_begin(369, 0x166UL);
	for (case_id = 0; case_id != 192; ++case_id) {
		if (case_id < 64) {
			expected = 0x5aa50000UL | ((case_id & 8U) ? 0x2468UL : 0x1357UL);
		} else if (case_id < 128) {
			expected = 0xa53c0000UL | (0x1111UL * ((case_id & 3U) + 1U));
		} else {
			expected = 0x5aa50000UL | ((case_id & 16U) ? 0x2468UL : 0x1357UL);
		}
		actual = llvm_entry_proxy(case_id);
		c166_test_check_u32(case_id + 1, expected, actual);
		if (actual != expected)
			break;
	}
	c166_test_finish();
	simulator_stop();
}
