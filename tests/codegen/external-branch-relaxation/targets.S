        .text
        .rept   256
        nop
        .endr

        .globl  _relax_zero
        .type   _relax_zero,@function
_relax_zero:
        mov     r4, #4660
        rets
        .size   _relax_zero, .-_relax_zero

        .globl  _relax_nonzero
        .type   _relax_nonzero,@function
_relax_nonzero:
        mov     r4, #22136
        rets
        .size   _relax_nonzero, .-_relax_nonzero
