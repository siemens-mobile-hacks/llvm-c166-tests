#include "constants.h"

.macro load_word reg, symbol
	exts #seg(\symbol), #1
	mov \reg, sof(\symbol)
.endm

.macro store_word symbol, reg
	exts #seg(\symbol), #1
	mov sof(\symbol), \reg
.endm

#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
#else
.text
#endif
.p2align 1
.globl _asm_stage
.type _asm_stage,@function
_asm_stage:
	mov [-r0], r6
	mov r6, r12
	load_word r13, _asm_bias
#if __C166_MEMORY_MODEL__ == 2
	calla cc_uc, cof(_c_mix)
#else
	calls seg(_c_mix), sof(_c_mix)
#endif
	cmp r6, #0
	jmpr cc_eq, 2f
	mov r1, #3
1:
	add r4, r6
	sub r1, #1
	jmpr cc_ne, 1b
2:
	load_word r1, _asm_calls
	add r1, #1
	store_word _asm_calls, r1
	mov r6, [r0+]
#if __C166_MEMORY_MODEL__ == 2
	ret
#else
	rets
#endif
.size _asm_stage, .-_asm_stage

#if __C166_MEMORY_MODEL__ == 3
.section .c166.small.data,"aw",@progbits
#else
.data
#endif
.p2align 1
.globl _asm_bias
.type _asm_bias,@object
_asm_bias:
	.word ASM_BIAS
.size _asm_bias, .-_asm_bias

#if __C166_MEMORY_MODEL__ == 3
.section .c166.small.bss,"aw",@nobits
#else
.bss
#endif
.p2align 1
.globl _asm_calls
.type _asm_calls,@object
_asm_calls:
	.zero 2
.size _asm_calls, .-_asm_calls
