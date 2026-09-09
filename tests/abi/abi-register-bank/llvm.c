typedef unsigned int u16;

volatile u16 interrupt_result
    __attribute__((c166_far, section(".c166.interrupt.data")));
volatile u16 interrupt_priority __attribute__((c166_far));
volatile u16 interrupt_marker __attribute__((c166_far));
extern void irq_window(u16 mode);
extern u16 pec_transfer(u16 mode);

__attribute__((noinline))
u16 interrupt_mix(u16 a, u16 b, u16 c, u16 d, u16 e) {
	return (u16)(a * b + c * d + e);
}

__attribute__((interrupt(-1), c166_register_bank("ISS_BANK"),
               section(".c166.interrupt.text")))
void llvm_interrupt(void) {
	u16 masked_request, equal_request, lower_request;
	volatile u16 locals[4];
	locals[0] = 3;
	locals[1] = 5;
	locals[2] = 7;
	locals[3] = 11;
	interrupt_result = 0xDEAD;
	__asm__ volatile("trap #123" ::: "memory");
	if (interrupt_result != 110) {
		interrupt_result = 0xE01B;
		return;
	}
	interrupt_result = 0xDEAD;
	/* Keep the priority-14 request pending until IEN and ILVL permit it. */
	__asm__ volatile(
		"push psw\n\t"
		"mov psw, #0\n\t"
		"push sfr(0xb1)\n\t"
		"mov sfr(0xb1), #0xf8\n\t"
		".rept 8\n\tnop\n\t.endr\n\t"
		"mov %0, sfr(0xb1)\n\t"
		"mov psw, #0xe800\n\t"
		".rept 8\n\tnop\n\t.endr\n\t"
		"mov %1, sfr(0xb1)\n\t"
		"mov psw, #0xf800\n\t"
		".rept 8\n\tnop\n\t.endr\n\t"
		"mov %2, sfr(0xb1)\n\t"
		"mov psw, #0x800\n\t"
		".rept 8\n\tnop\n\t.endr\n\t"
		"bclr psw.11\n\t"
		"pop sfr(0xb1)\n\t"
		"pop psw\n\t"
		"nop"
		: "=&r"(masked_request), "=&r"(equal_request), "=&r"(lower_request)
		: : "memory", "cc");
	if (!(masked_request & 0x80)) {
		interrupt_result = 0xE01E;
		return;
	}
	if (!(equal_request & 0x80)) {
		interrupt_result = 0xE01F;
		return;
	}
	if (!(lower_request & 0x80)) {
		interrupt_result = 0xE020;
		return;
	}
	if (interrupt_result != 110) {
		interrupt_result = 0xE01C;
		return;
	}
	if (interrupt_priority != 14) {
		interrupt_result = 0xE01D;
		return;
	}
	interrupt_marker = 0xFFFF;
	irq_window(0);
	if (interrupt_marker >= 3) {
		interrupt_result = 0xE021;
		return;
	}
	for (u16 count = 1; count <= 4; ++count) {
		for (u16 mode = 1; mode <= 10; ++mode) {
			u16 id = mode + 16 * (count - 1);
			interrupt_marker = 0xFFFF;
			irq_window(id);
			if (interrupt_marker != count - 1) {
				interrupt_result = 0xE030 + id;
				return;
			}
		}
	}
	for (u16 mode = 0; mode <= 11; ++mode) {
		u16 pec_result = pec_transfer(mode);
		if (pec_result != 0) {
			interrupt_result = 0xE080 + 8 * mode + pec_result;
			return;
		}
	}
	interrupt_result =
		interrupt_mix(locals[0], locals[1], locals[2], locals[3], 13);
}

__attribute__((interrupt(-1), c166_register_bank("ISS_NESTED_BANK"),
               section(".c166.interrupt.nested.text")))
void llvm_nested_interrupt(void) {
	u16 status;
	volatile u16 locals[4];
	__asm__ volatile("mov %0, psw" : "=r"(status));
	interrupt_priority = status >> 12;
	__asm__ volatile("mov %0, mdh" : "=r"(status));
	interrupt_marker = status;
	locals[0] = 4;
	locals[1] = 5;
	locals[2] = 7;
	locals[3] = 11;
	interrupt_result =
		interrupt_mix(locals[0], locals[1], locals[2], locals[3], 13);
}

u16 llvm_anchor(void) {
	return 0;
}
