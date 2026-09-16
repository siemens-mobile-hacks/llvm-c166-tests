#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_shift_rotate
#define RETURN ret
#else
.text
.c166_function huge, _test_shift_rotate
#define RETURN rets
#endif
.globl _test_shift_rotate
.type _test_shift_rotate,@function
_test_shift_rotate:
  push psw
  mov r4, r12
  mov r1, r13

.macro register_form id, mnemonic, count
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  \mnemonic r4, \count
  mov r5, psw
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  register_form 0, rol, r13
  register_form 1, ror, r13
  register_form 2, shl, r13
  register_form 3, shr, r13
  register_form 4, ashr, r13
  register_form 10, rol, r4
  register_form 11, ror, r4
  register_form 12, shl, r4
  register_form 13, shr, r4
  register_form 14, ashr, r4

.macro immediate_value mnemonic, count
  cmp r13, #\count
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  \mnemonic r4, #\count
  mov r5, psw
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
.macro immediate_form id, mnemonic
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  .irp count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    immediate_value \mnemonic, \count
  .endr
.Lnext\@:
.endm
  immediate_form 5, rol
  immediate_form 6, ror
  immediate_form 7, shl
  immediate_form 8, shr
  immediate_form 9, ashr
  mov r5, #0xffff
.Ldone:
  and r5, #31
  cmp r13, r1
  jmpr cc_eq, .Lreturn
  or r5, #32
.Lreturn:
  pop psw
  RETURN
.size _test_shift_rotate, .-_test_shift_rotate
