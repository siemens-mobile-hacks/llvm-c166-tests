#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned long llvm_entry_proxy(unsigned int lhs, unsigned int rhs,
	unsigned int flags, unsigned int form);

static const unsigned int words[] = {
	0, 0xffff, 0x5555, 0xaaaa, 1, 0x8000, 0x7fff, 0xfffe
};

static unsigned long expected(unsigned int lhs, unsigned int rhs, unsigned int form) {
	unsigned int operation = form / 5;
	unsigned int placement = form % 5;
	unsigned int mask = placement == 1 || placement == 4 ? 0x8000 : 1;
	unsigned int a = (lhs & mask) != 0;
	unsigned int b = placement == 0 || placement == 3 ? (rhs >> 15) : (lhs & 1);
	unsigned int result_bit = a;
	unsigned int flags;

	switch (operation) {
		case 0:
			result_bit = b;
			break;
		case 1:
			result_bit = !b;
			break;
		case 2:
			result_bit = a & b;
			break;
		case 3:
			result_bit = a | b;
			break;
		case 4:
			result_bit = a ^ b;
			break;
	}
	if (operation < 2) {
		flags = b ? 1 : 8;
	} else {
		flags = (a ^ b) | ((a & b) << 1) | ((a | b) << 2) | (!(a | b) << 3);
	}
	return ((unsigned long)flags << 16) | ((lhs & ~mask) | (result_bit ? mask : 0));
}

void main(void) {
	unsigned int form, left, right, flags, id = 0;
	unsigned long want, actual;

	c166_test_begin(381, 0x166UL);
	for (form = 0; form < 30; ++form) {
		for (left = 0; left < 8; ++left) {
			for (right = 0; right < 8; ++right) {
				want = expected(words[left], words[right], form);
				for (flags = 0; flags < 32; ++flags) {
					c166_test_set_context(0x166UL, form, flags, words[left], words[right]);
					actual = llvm_entry_proxy(words[left], words[right], flags, form);
					c166_test_check_u32(++id, want, actual);
					if (actual != want)
						goto done;
				}
			}
		}
	}
done:
	c166_test_finish();
	simulator_stop();
}
