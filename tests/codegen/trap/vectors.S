.macro trap_vector number
.section .c166.vector.\number,"ax",@progbits
.globl _trap_vector_\number
.type _trap_vector_\number,@function
_trap_vector_\number:
  jmps seg(_trap_handler), sof(_trap_handler)
.size _trap_vector_\number, .-_trap_vector_\number
.endm

trap_vector 1
trap_vector 63
trap_vector 127
