#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_bit_binary
#define RETURN ret
#else
.text
.c166_function huge, _test_bit_binary
#define RETURN rets
#endif
.globl _test_bit_binary
.equ RAM_FIRST, 0
.equ RAM_LAST, 127
.type _test_bit_binary,@function
_test_bit_binary:
  push psw
  push dpp3
  push r6
  push r7
  push r8
  push r9
  mov dpp3, #3
  mov r6, 0xfd00
  mov r7, 0xfdfe
  mov r8, 0xfd02
  mov r9, 0xfdfc
  mov 0xfd00, r12
  mov 0xfdfe, r13
  mov r1, #0xa55a
  mov 0xfd02, r1
  mov 0xfdfc, r1
  mov r4, r12
  mov r5, r13

.macro exercise id, mnemonic, dst, src, memory=0
  cmp r15, #\id
  jmpr cc_ne, .Lnext\@
  mov psw, r14
  \mnemonic \dst, \src
  mov r1, psw
  .if \memory
    mov r4, 0xfd00
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
.macro operation id, mnemonic
  exercise (5*\id), \mnemonic, r4.0, r5.15
  exercise (5*\id+1), \mnemonic, r4.15, r4.0
  exercise (5*\id+2), \mnemonic, r4.0, r4.0
  exercise (5*\id+3), \mnemonic, RAM_FIRST.0, RAM_LAST.15, 1
  exercise (5*\id+4), \mnemonic, RAM_FIRST.15, RAM_FIRST.0, 1
.endm
  operation 0, bmov
  operation 1, bmovn
  operation 2, band
  operation 3, bor
  operation 4, bxor
  operation 5, bcmp
  mov r1, #0xffff
.Ldone:
  and r1, #31
  cmp r5, r13
  jmpr cc_eq, .Lsource_ok
  or r1, #32
.Lsource_ok:
  mov r2, 0xfdfe
  cmp r2, r13
  jmpr cc_eq, .Lmemory_source_ok
  or r1, #32
.Lmemory_source_ok:
  mov r2, 0xfd02
  cmp r2, #0xa55a
  jmpr cc_ne, .Lcanary_error
  mov r2, 0xfdfc
  cmp r2, #0xa55a
  jmpr cc_eq, .Lrestore
.Lcanary_error:
  or r1, #64
.Lrestore:
  mov 0xfd00, r6
  mov 0xfdfe, r7
  mov 0xfd02, r8
  mov 0xfdfc, r9
  mov r5, r1
  pop r9
  pop r8
  pop r7
  pop r6
  pop dpp3
  pop psw
  RETURN
.size _test_bit_binary, .-_test_bit_binary
