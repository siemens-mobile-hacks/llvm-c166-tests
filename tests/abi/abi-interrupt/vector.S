.section .c166.vector.124,"ax",@progbits
.globl _interrupt_vector
.type _interrupt_vector,@function
_interrupt_vector:
  jmps seg(_interrupt_handler), sof(_interrupt_handler)
.size _interrupt_vector, .-_interrupt_vector
