#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int index,
	unsigned int flags, unsigned int form);

static const unsigned int words[] = {
	0, 0xffff, 0x5555, 0xaaaa, 1, 0x8000, 0x7fff, 0xfffe
};

void main(void) {
	unsigned int form, index, word, initial, mask, value, flags, result, id = 0;
	unsigned long expected, actual;

	c166_test_begin(382, 0x166UL);
	for (form = 0; form < 6; ++form) {
		for (index = 0; index < 16; ++index) {
			mask = 1U << index;
			for (word = 0; word < 8; ++word) {
				value = words[word];
				flags = (value & mask) ? 1 : 8;
				result = form & 1 ? value | mask : value & ~mask;
				expected = ((unsigned long)flags << 16) | result;
				for (initial = 0; initial < 32; ++initial) {
					c166_test_set_context(0x166UL, form, index, value, initial);
					actual = llvm_entry_proxy(value, index, initial, form);
					c166_test_check_u32(++id, expected, actual);
					if (actual != expected)
						goto done;
				}
			}
		}
	}
done:
	c166_test_finish();
	simulator_stop();
}
