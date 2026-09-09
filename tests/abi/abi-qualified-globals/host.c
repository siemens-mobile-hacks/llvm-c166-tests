#include "c166-test-runtime.h"
volatile unsigned int simulator_result;

extern unsigned long llvm_entry_proxy(unsigned int seed, unsigned int read_only);
extern unsigned long c166_qualified_globals(unsigned int seed, unsigned int read_only);
extern void llvm_crt_init_proxy(void);

static const unsigned int seeds[] = {
	0x0000U, 0x0001U, 0x1234U, 0x7fffU,
	0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
};

void main(void) {
	unsigned int index;

	simulator_result = 42U;
	if (c166_qualified_globals(0, 1) != 0) {
		simulator_result = 0x10U;
		simulator_stop();
		return;
	}
	if (llvm_entry_proxy(0, 1) != 0) {
		simulator_result = 0x11U;
		simulator_stop();
		return;
	}
	for (index = 0; index != 8; ++index) {
		unsigned long expected = c166_qualified_globals(seeds[index], 0);
		unsigned long actual = llvm_entry_proxy(seeds[index], 0);
		if (actual != expected) {
			simulator_result = (unsigned int)(0x100U + index);
			break;
		}
		if (llvm_entry_proxy(0, 1) == 0) {
			simulator_result = (unsigned int)(0x200U + index);
			break;
		}
		llvm_crt_init_proxy();
		if (llvm_entry_proxy(0, 1) != 0) {
			simulator_result = (unsigned int)(0x300U + index);
			break;
		}
	}
	simulator_stop();
}
