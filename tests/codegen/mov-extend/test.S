#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_mov_extend
#define RETURN ret
#else
.text
.c166_function huge, _test_mov_extend
#define RETURN rets
#endif
.globl _test_mov_extend
.type _test_mov_extend,@function
_test_mov_extend:
  push psw
  push mdc
  push mdh
  push mdl
  push dpp1
  mov dpp1, #pag(.Lbuffer)
  mov r1, r12
  or r1, #0xa500
  mov r2, r12
  shl r2, #8
  or r2, #0x5a
  mov 0x4002, r2
  mov r4, r12
  xor r4, #0xffff
  mov 0x4004, r4

.macro transfer id, mnemonic, kind
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  .if \kind == 2
    mov mdl, r4
  .else
    mov mdl, r1
  .endif
  mov psw, r13
  .if \kind == 0
    \mnemonic r4, rl1
  .elseif \kind == 1
    \mnemonic r4, 0x4003
  .elseif \kind == 2
    \mnemonic mdl, 0x4003
  .elseif \kind == 3
    \mnemonic 0x4004, rl1
  .else
    \mnemonic 0x4004, mdl
  .endif
  mov r5, psw
  and r5, #31
  .if \kind == 2
    mov r4, mdl
  .else
    mov r3, mdl
    cmp r3, r1
    jmpr cc_ne, .Lbad
  .endif
  .if \kind >= 3
    mov r4, 0x4004
  .else
    mov r3, 0x4004
    xor r3, #0xffff
    cmp r3, r12
    jmpr cc_ne, .Lbad
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  transfer 0, movbs, 0
  transfer 1, movbs, 1
  transfer 2, movbs, 2
  transfer 3, movbs, 3
  transfer 4, movbs, 4
  transfer 5, movbz, 0
  transfer 6, movbz, 1
  transfer 7, movbz, 2
  transfer 8, movbz, 3
  transfer 9, movbz, 4
  mov r5, #32
  jmpr cc_uc, .Lreturn
.Ldone:
  mov r3, 0x4002
  cmp r3, r2
  jmpr cc_ne, .Lbad
  mov r3, r1
  xor r3, #0xa500
  cmp r3, r12
  jmpr cc_ne, .Lbad
  mov r3, 0x4000
  cmp r3, #0xc35a
  jmpr cc_ne, .Lbad
  mov r3, 0x4006
  cmp r3, #0x5ac3
  jmpr cc_eq, .Lreturn
.Lbad:
  or r5, #32
.Lreturn:
  pop dpp1
  pop mdl
  pop mdh
  pop mdc
  pop psw
  RETURN
.size _test_mov_extend, .-_test_mov_extend

.section .mov.buffer,"aw",@progbits
.p2align 1
.Lbuffer:
  .short 0xc35a, 0, 0, 0x5ac3
