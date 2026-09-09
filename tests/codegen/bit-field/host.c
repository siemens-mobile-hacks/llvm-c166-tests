#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int value, unsigned int vector,
	unsigned int flags, unsigned int form);

static const unsigned int words[] = {0, 0xffff, 0x8000, 0x007f};
static const unsigned int bytes[] = {0, 255, 85, 170, 1, 128, 127, 254};

void main(void) {
	unsigned int form, mask, data, word, initial, shift, value, result, flags, id = 0;
	unsigned long expected, actual;

	c166_test_begin(383, 0x166UL);
	for (form = 0; form < 6; ++form) {
		shift = (form & 1) * 8;
		for (mask = 0; mask < 8; ++mask) {
			for (data = 0; data < 8; ++data) {
				for (word = 0; word < 4; ++word) {
					value = words[word];
					result = (value & ~(bytes[mask] << shift)) | (bytes[data] << shift);
					flags = (result >> 15) | (result == 0 ? 8 : 0);
					expected = ((unsigned long)flags << 16) | result;
					for (initial = 0; initial < 32; ++initial) {
						c166_test_set_context(0x166UL, form, mask * 8 + data, value, initial);
						actual = llvm_entry_proxy(value, mask * 8 + data, initial, form);
						c166_test_check_u32(++id, expected, actual);
						if (actual != expected)
							goto done;
					}
				}
			}
		}
	}
done:
	c166_test_finish();
	simulator_stop();
}
