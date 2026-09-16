#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
        .section .c166.near.text,"ax",@progbits
        .c166_function near, _relax_probe
#else
        .text
        .c166_function huge, _relax_probe
#endif
        .globl  _relax_probe
        .type   _relax_probe,@function
_relax_probe:
        mov     r1, #0
        cmp     r12, r1
        jmpr    cc_eq, _relax_zero
        jmpr    cc_uc, _relax_nonzero
        .size   _relax_probe, .-_relax_probe
