#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
        .section .c166.near.text,"ax",@progbits
#define RETURN ret
#else
        .text
#define RETURN rets
#endif
        .rept   256
        nop
        .endr

        .globl  _relax_zero
        .type   _relax_zero,@function
_relax_zero:
        mov     r4, #4660
        RETURN
        .size   _relax_zero, .-_relax_zero

        .globl  _relax_nonzero
        .type   _relax_nonzero,@function
_relax_nonzero:
        mov     r4, #22136
        RETURN
        .size   _relax_nonzero, .-_relax_nonzero
