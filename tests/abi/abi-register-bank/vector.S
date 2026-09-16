.section .c166.vector.35,"ax",@progbits
.globl _register_bank_irq_vector
.type _register_bank_irq_vector,@function
_register_bank_irq_vector:
  jmps seg(_nested_register_bank_interrupt), sof(_nested_register_bank_interrupt)
.size _register_bank_irq_vector, .-_register_bank_irq_vector

.section .c166.vector.123,"ax",@progbits
.globl _register_bank_nested_vector
.type _register_bank_nested_vector,@function
_register_bank_nested_vector:
  jmps seg(_nested_register_bank_interrupt), sof(_nested_register_bank_interrupt)
.size _register_bank_nested_vector, .-_register_bank_nested_vector

.section .c166.vector.124,"ax",@progbits
.globl _register_bank_trap_vector
.type _register_bank_trap_vector,@function
_register_bank_trap_vector:
  jmps seg(_register_bank_interrupt), sof(_register_bank_interrupt)
.size _register_bank_trap_vector, .-_register_bank_trap_vector
