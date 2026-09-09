#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int flags,
	unsigned int form);

static const unsigned int words[] = {
	0, 1, 2, 0x7fff, 0x8000, 0xffff, 0xa55a, 0x5aa5
};
static const unsigned int register_kinds[] = {0, 1, 2, 3, 7, 8};

void main(void) {
	unsigned int form, index, initial, value, result, flags, sign, kind, count, id;
	unsigned int high_result;
	unsigned long want, actual;

	c166_test_begin(385, 0x166UL);
	for (form = 0; form != 39; ++form) {
		kind = form < 27 ? form % 9 : register_kinds[(form - 27) % 6];
		if (kind == 0 || kind == 1 || kind == 7) {
			high_result = form >= 27;
		} else {
			high_result = (form >= 18 && form < 27) || form >= 33;
		}
		count = form < 9 ? sizeof(words) / sizeof(words[0]) : 256;
		sign = form < 9 ? 0x8000U : 0x80U;
		id = 0;
		for (index = 0; index != count; ++index) {
			value = form < 9 ? words[index] : index;
			result = value;
			if (form >= 9) {
				if (high_result) {
					result = (value << 8) | 0xc3U;
				} else {
					result |= 0x3c00U;
				}
			}
			for (initial = 0; initial != 32; ++initial) {
				flags = initial & 6U;
				if (value & sign)
					flags |= 1U;
				if (value == 0)
					flags |= 8U;
				if (value == sign)
					flags |= 16U;
				want = ((unsigned long)flags << 16) | result;
				c166_test_set_context(0x166UL, form, initial, value, 0);
				actual = llvm_entry_proxy(value, initial, form);
				c166_test_check_u32(++id, want, actual);
				if (actual != want)
					goto done;
			}
		}
	}
done:
	c166_test_finish();
	simulator_stop();
}
