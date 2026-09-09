#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_mov_memory
#define RETURN ret
#else
.text
.c166_function huge, _test_mov_memory
#define RETURN rets
#endif
.globl _test_mov_memory
.type _test_mov_memory,@function
_test_mov_memory:
  push psw
  push mdc
  push mdh
  push mdl
  push dpp1
  mov dpp1, #pag(.Lbuffer)
  mov 0x4002, r12
  mov r2, r12
  xor r2, #0xffff
  mov 0x4004, r2

.macro transfer id, sfr, store=0
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  .if \store
    mov \sfr, r12
    mov psw, r13
    mov 0x4004, \sfr
  .else
    mov \sfr, r2
    mov psw, r13
    mov \sfr, 0x4002
  .endif
  mov r5, psw
  mov r3, \sfr
  .if \store
    mov r4, 0x4004
  .else
    mov r4, r3
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  transfer 0, mdl
  transfer 1, mdl, 1
  transfer 2, mdh
  transfer 3, mdh, 1

// All memory destinations start with ~value. Byte forms write the high byte
// at an odd address, allowing the unchanged low byte to be checked on return.
.macro memory_transfer id, mnemonic, to_indirect=0, byte=0, predecrement=0
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  mov r3, r12
  .if \predecrement
    mov r1, r12
    mov r2, #0x4006
    mov psw, r13
    movb [-r2], rh1
  .elseif \to_indirect
    mov r2, #(0x4004 + \byte)
    mov psw, r13
    \mnemonic [r2], (0x4002 + \byte)
  .else
    mov r2, #(0x4002 + \byte)
    mov psw, r13
    \mnemonic (0x4004 + \byte), [r2]
  .endif
  mov r5, psw
  and r5, #31
  .if \predecrement
    cmp r2, #0x4005
    jmpr cc_ne, .Lbad
    cmp r1, r12
  .else
    cmp r2, #(0x4002 + 2 * \to_indirect + \byte)
  .endif
  jmpr cc_ne, .Lbad
  mov r4, 0x4004
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  memory_transfer 4, mov, 1
  memory_transfer 5, mov
  memory_transfer 6, movb, 1, 1
  memory_transfer 7, movb, 0, 1
  memory_transfer 8, movb, 0, 1, 1

.macro byte_direct id, kind
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  mov r3, r12
  mov r4, r2
  mov mdl, r2
  mov r1, mdl
  mov r5, #0
  xor r1, r12
  cmp r1, #0xffff
  jmpr cc_ne, .Lbad
  mov r1, r12
    // MDL destinations exercise the architectural rule that byte writes to
    // an SFR clear the complementary high byte, for every available source.
    .if \kind == 4
    shr r1, #8
    .irp value, 0, 1, 2, 90, 127, 128, 165, 254, 255
      cmp r1, #\value
      jmpr cc_ne, 1f
      mov psw, r13
      movb mdl, #\value
      mov r5, psw
      mov r4, mdl
      jmpr cc_uc, .Ldone
1:
    .endr
  .else
    rol r1, #8
    mov mdh, r1
    .if \kind == 2
      mov mdl, r1
    .endif
    mov psw, r13
    .if \kind == 0
      movb rl4, 0x4003
    .elseif \kind == 1
      movb mdl, 0x4003
    .elseif \kind == 2
      movb 0x4005, mdl
    .elseif \kind == 3
      movb mdl, mdh
    .else
      movb rh4, 0x4003
    .endif
    mov r5, psw
    and r5, #31
    .if \kind == 2
      mov r2, mdl
      cmp r2, r1
      jmpr cc_ne, .Lbad
      mov r4, 0x4004
    .elseif \kind == 1 || \kind == 3
      mov r4, mdl
    .endif
    mov r2, mdh
    cmp r2, r1
    jmpr cc_ne, .Lbad
    jmpr cc_uc, .Ldone
  .endif
.Lnext\@:
.endm
  byte_direct 9, 0
  byte_direct 10, 1
  byte_direct 11, 2
  byte_direct 12, 3
  byte_direct 13, 4
  byte_direct 14, 5
  mov r5, #0xffff
.Ldone:
  and r5, #31
  cmp r3, r12
  jmpr cc_ne, .Lbad
  mov r2, 0x4002
  cmp r2, r12
  jmpr cc_ne, .Lbad
  mov r2, 0x4000
  cmp r2, #0xc35a
  jmpr cc_ne, .Lbad
  mov r2, 0x4006
  cmp r2, #0x5ac3
  jmpr cc_ne, .Lbad
  cmp r14, #9
  jmpr cc_ult, .Lcheck_old_destination
  cmp r14, #11
  jmpr cc_eq, .Lreturn
  jmpr cc_uc, .Lcheck_destination
.Lcheck_old_destination:
  cmp r14, #4
  jmpr cc_uge, .Lreturn
  and r14, #1
  jmpr cc_ne, .Lreturn
.Lcheck_destination:
  mov r2, 0x4004
  xor r2, #0xffff
  cmp r2, r12
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
.size _test_mov_memory, .-_test_mov_memory

.section .mov.buffer,"aw",@progbits
.p2align 1
.Lbuffer:
  .short 0xc35a, 0, 0, 0x5ac3
