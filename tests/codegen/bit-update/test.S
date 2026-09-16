#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_bit_update
#define RETURN ret
#else
.text
.c166_function huge, _test_bit_update
#define RETURN rets
#endif
.globl _test_bit_update
.type _test_bit_update,@function
_test_bit_update:
  push psw
  push dpp3
  push r6
  push r7
  push r8
  push r9
  mov dpp3, #3
  mov r6, #0xfd00
  mov r7, #0xfd02
  cmp r15, #4
  jmpr cc_ult, .Laddress_ready
  mov r6, #0xfdfe
  mov r7, #0xfdfc
.Laddress_ready:
  mov r8, [r6]
  mov r9, [r7]
  mov [r6], r12
  mov r1, #0xa55a
  mov [r7], r1
  mov r4, r12

.macro form id, mnemonic, base
  cmp r15, #\id
  jmpr cc_ne, .Lnext\@
  .irp index, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    cmp r13, #\index
    jmpr cc_ne, 1f
    mov psw, r14
    \mnemonic \base . \index
    mov r5, psw
    and r5, #31
    jmpr cc_uc, .Ldone
1:
  .endr
.Lnext\@:
.endm
  form 0, bclr, r4
  form 1, bset, r4
  form 2, bclr, 0
  form 3, bset, 0
  form 4, bclr, 127
  form 5, bset, 127
  mov r5, #64
.Ldone:
  cmp r15, #2
  jmpr cc_ult, .Lresult_ready
  mov r4, [r6]
.Lresult_ready:
  mov r1, [r7]
  cmp r1, #0xa55a
  jmpr cc_eq, .Lrestore
  or r5, #64
.Lrestore:
  mov [r6], r8
  mov [r7], r9
  pop r9
  pop r8
  pop r7
  pop r6
  pop dpp3
  pop psw
  RETURN
.size _test_bit_update, .-_test_bit_update
