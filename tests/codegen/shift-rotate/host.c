#include "c166-test-result.h"
#include "c166-test-runtime.h"

extern unsigned long llvm_entry_proxy(
    unsigned int value, unsigned int count, unsigned int form, unsigned int flags);
static const unsigned int words[] = {0, 1, 2, 0x7fff, 0x8000, 0xffff, 0xa55a, 0x5aa5};
static const unsigned int upper[] = {0, 0x10, 0x8000, 0xfff0};

enum { ROL, ROR, SHL, SHR, ASHR, OP_COUNT };

/* Right shifts and ROR accumulate the previous cycle's C into rounding V. */
static unsigned long expected(unsigned int word, unsigned int count, unsigned int operation) {
	unsigned int carry = 0, overflow = 0, flags;
	count &= 15U;
	while (count--) {
		if (operation == ROR || operation == SHR || operation == ASHR) {
			overflow |= carry;
			carry = word & 1U;
			if (operation == ROR) {
				word = (word >> 1) | (carry << 15);
			} else if (operation == ASHR) {
				word = (word >> 1) | (word & 0x8000U);
			} else {
				word >>= 1;
			}
		} else {
			carry = word >> 15;
			word = (word << 1) | (operation == ROL ? carry : 0);
		}
	}
	flags = (word >> 15) | (carry << 1) | (overflow << 2);
	if (word == 0)
		flags |= 8U;
	return ((unsigned long)flags << 16) | word;
}

void main(void) {
	unsigned int form, value, index, limit, count, flags, id = 0;
	unsigned long want, actual;
	c166_test_begin(371, 0x166UL);
	for (form = 0; form != 3 * OP_COUNT; ++form) {
		id = 0;
		limit = form < OP_COUNT ? 64 : (form < 2 * OP_COUNT ? 16 : 1);
		for (value = 0; value != 8; ++value)
			for (index = 0; index != limit; ++index) {
				count = form < OP_COUNT ? upper[index / 16] | (index & 15U) : index;
				if (form >= 2 * OP_COUNT)
					count = words[value];
				want = expected(words[value], count, form % OP_COUNT);
				for (flags = 0; flags != 32; ++flags) {
					c166_test_set_context(0x166UL, form, flags, words[value], count);
					actual = llvm_entry_proxy(words[value], count, form, flags);
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
