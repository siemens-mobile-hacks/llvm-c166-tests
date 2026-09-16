#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_bit_field
#define RETURN ret
#else
.text
.c166_function huge, _test_bit_field
#define RETURN rets
#endif
.globl _test_bit_field
.type _test_bit_field,@function
_test_bit_field:
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
  .set .Lvector\@, 0
  .irp mask, 0, 255, 85, 170, 1, 128, 127, 254
    .irp data, 0, 255, 85, 170, 1, 128, 127, 254
      cmp r13, #.Lvector\@
      jmpr cc_ne, 1f
      mov psw, r14
      \mnemonic \base, #\mask, #\data
      mov r5, psw
      and r5, #31
      jmpr cc_uc, .Ldone
1:
      .set .Lvector\@, .Lvector\@ + 1
    .endr
  .endr
.Lnext\@:
.endm
  form 0, bfldl, r4
  form 1, bfldh, r4
  form 2, bfldl, 0
  form 3, bfldh, 0
  form 4, bfldl, 127
  form 5, bfldh, 127
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
.size _test_bit_field, .-_test_bit_field
