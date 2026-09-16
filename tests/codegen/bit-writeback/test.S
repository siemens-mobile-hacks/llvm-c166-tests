#ifndef TEST_FUNCTION_SYMBOL
#define TEST_FUNCTION_SYMBOL _test_bit_writeback
#endif

#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, TEST_FUNCTION_SYMBOL
#define RETURN ret
#else
.text
.c166_function huge, TEST_FUNCTION_SYMBOL
#define RETURN rets
#endif
#ifdef BIT_BRANCH_READ_ONLY
#define BRANCH_SET jb
#define BRANCH_CLEAR jnb
#else
#define BRANCH_SET jbc
#define BRANCH_CLEAR jnbs
#endif
.globl TEST_FUNCTION_SYMBOL
.type TEST_FUNCTION_SYMBOL,@function
TEST_FUNCTION_SYMBOL:
  push psw
  push dpp3
  push r7
  push r8
  push r9
  push r10
  mov dpp3, #3
  mov r7, #0xfd00
  mov r8, #0xfd02
  cmp r15, #8
  jmpr cc_ult, .Laddress_ready
  mov r7, #0xfdfe
  mov r8, #0xfdfc
.Laddress_ready:
  mov r9, [r7]
  mov r10, [r8]
  mov [r7], r12
  mov r1, #0xa55a
  mov [r8], r1
  mov r4, r12

.macro branch bit, mnemonic, long_form, base
  cmp r13, #\bit
  jmpr cc_ne, .Lnext\@
  mov psw, r14
  .if \long_form
    \mnemonic \base . .Lbit\@, .Llong_taken
  .else
    \mnemonic \base . .Lbit\@, .Ltaken\@
  .endif
  .equ .Lbit\@, \bit
  mov r5, psw
  and r5, #31
  jmpr cc_uc, .Ldone
  .if !\long_form
.Ltaken\@:
  mov r5, psw
  and r5, #31
  or r5, #32
  jmpr cc_uc, .Ldone
  .endif
.Lnext\@:
.endm
.macro form id, mnemonic, long_form=0, base=r4
  cmp r15, #\id
  jmpr cc_ne, .Lnext\@
  .irp bit, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    branch \bit, \mnemonic, \long_form, \base
  .endr
.Lnext\@:
.endm
  form 0, BRANCH_SET
  form 1, BRANCH_CLEAR
  form 2, BRANCH_SET, 1
  form 3, BRANCH_CLEAR, 1
  // RAM bitoff boundaries: FD00 + 2 * bitoff, independent of DPP/SFR names.
  form 4, BRANCH_SET, 0, 0
  form 5, BRANCH_CLEAR, 0, 0
  form 6, BRANCH_SET, 1, 0
  form 7, BRANCH_CLEAR, 1, 0
  form 8, BRANCH_SET, 0, 127
  form 9, BRANCH_CLEAR, 0, 127
  form 10, BRANCH_SET, 1, 127
  form 11, BRANCH_CLEAR, 1, 127
  mov r5, #64
  jmpr cc_uc, .Ldone
  // One shared distant target keeps every long branch out of range without
  // duplicating padding for each bit. The padding is never executed.
  .space 260
.Llong_taken:
  mov r5, psw
  and r5, #31
  or r5, #32
.Ldone:
  cmp r15, #4
  jmpr cc_ult, .Lresult_ready
  mov r4, [r7]
.Lresult_ready:
  mov r1, [r8]
  cmp r1, #0xa55a
  jmpr cc_eq, .Lcanary_ok
  or r5, #64
.Lcanary_ok:
  mov [r7], r9
  mov [r8], r10
  pop r10
  pop r9
  pop r8
  pop r7
  pop dpp3
  pop psw
  RETURN
.size TEST_FUNCTION_SYMBOL, .-TEST_FUNCTION_SYMBOL
