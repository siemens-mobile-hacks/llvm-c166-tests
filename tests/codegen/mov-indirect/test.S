#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_mov_indirect
#define RETURN ret
#else
.text
.c166_function huge, _test_mov_indirect
#define RETURN rets
#endif
.globl _test_mov_indirect
.type _test_mov_indirect,@function
_test_mov_indirect:
  push psw
  push dpp1
  mov dpp1, #pag(.Lbuffer)
  mov r3, r12
  mov r15, r12
  cmp r14, #9
  jmpr cc_ult, .Lsource_ready
  cmp r14, #18
  jmpr cc_ult, .Leven_source
  cmp r14, #27
  jmpr cc_ult, .Lodd_source
  shl r3, #8
  or r3, #0x5a
  cmp r14, #33
  jmpr cc_uge, .Lodd_source
.Leven_source:
  or r15, #0xa500
  jmpr cc_uc, .Lsource_ready
.Lodd_source:
  shl r15, #8
  or r15, #0x5a
.Lsource_ready:
  mov 0x4002, r15
  mov r4, #0x3cc3
  mov 0x4006, r4

// kind: load, load+, store, -store, copy, dst+copy, src+copy,
// displaced load, displaced store. Each branch checks both pointer results.
.macro transfer id, kind, op, dst, src, step, offset
  cmp r14, #\id
  jmpr cc_ne, .Lnext\@
  .set source\@, 0x4002 + \offset
  .set destination\@, 0x4006 + \offset
  .if \kind == 7
    .set source\@, source\@ - 2
  .elseif \kind == 8
    .set destination\@, destination\@ - 2
  .endif
  mov r1, #source\@
  mov r2, #destination\@
  .if \kind == 3
    add r2, #\step
  .endif
  mov psw, r13
  .if \kind == 0
    \op \dst, [r1]
  .elseif \kind == 1
    \op \dst, [r1+]
  .elseif \kind == 2
    \op [r2], \src
  .elseif \kind == 3
    \op [-r2], \src
  .elseif \kind == 4
    \op [r2], [r1]
  .elseif \kind == 5
    \op [r2+], [r1]
  .elseif \kind == 6
    \op [r2], [r1+]
  .elseif \kind == 7
    \op \dst, [r1 + #2]
  .else
    \op [r2 + #2], \src
  .endif
  mov r5, psw
  and r5, #31
  .if (\kind == 1) || (\kind == 6)
    .set source\@, source\@ + \step
  .elseif \kind == 5
    .set destination\@, destination\@ + \step
  .endif
  cmp r1, #source\@
  jmpr cc_ne, .Lbad
  cmp r2, #destination\@
  jmpr cc_ne, .Lbad
  .if (\kind == 0) || (\kind == 1) || (\kind == 7)
    mov r2, 0x4006
    cmp r2, #0x3cc3
    jmpr cc_ne, .Lbad
  .else
    mov r4, 0x4006
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm

.irp kind, 0, 1, 2, 3, 4, 5, 6, 7, 8
  transfer \kind, \kind, mov, r4, r3, 2, 0
.endr
.irp kind, 0, 1, 2, 3, 4, 5, 6, 7, 8
  transfer (9+\kind), \kind, movb, rl4, rl3, 1, 0
.endr
.irp kind, 0, 1, 2, 3, 4, 5, 6, 7, 8
  transfer (18+\kind), \kind, movb, rl4, rl3, 1, 1
.endr
// Repeat only register forms for RH; memory-to-memory forms have no byte GPR.
  transfer 27, 0, movb, rh4, rh3, 1, 0
  transfer 28, 1, movb, rh4, rh3, 1, 0
  transfer 29, 2, movb, rh4, rh3, 1, 0
  transfer 30, 3, movb, rh4, rh3, 1, 0
  transfer 31, 7, movb, rh4, rh3, 1, 0
  transfer 32, 8, movb, rh4, rh3, 1, 0
  transfer 33, 0, movb, rh4, rh3, 1, 1
  transfer 34, 1, movb, rh4, rh3, 1, 1
  transfer 35, 2, movb, rh4, rh3, 1, 1
  transfer 36, 3, movb, rh4, rh3, 1, 1
  transfer 37, 7, movb, rh4, rh3, 1, 1
  transfer 38, 8, movb, rh4, rh3, 1, 1
  mov r5, #32
  jmpr cc_uc, .Lreturn
.Ldone:
  mov r2, r12
  cmp r14, #27
  jmpr cc_ult, .Lcheck_source
  shl r2, #8
  or r2, #0x5a
.Lcheck_source:
  cmp r3, r2
  jmpr cc_ne, .Lbad
  mov r2, 0x4002
  cmp r2, r15
  jmpr cc_ne, .Lbad
  mov r2, 0x4000
  cmp r2, #0xc35a
  jmpr cc_ne, .Lbad
  mov r2, 0x4004
  cmp r2, #0xa55a
  jmpr cc_ne, .Lbad
  mov r2, 0x4008
  cmp r2, #0x5ac3
  jmpr cc_eq, .Lreturn
.Lbad:
  or r5, #32
.Lreturn:
  pop dpp1
  pop psw
  RETURN
.size _test_mov_indirect, .-_test_mov_indirect

.section .mov.buffer,"aw",@progbits
.p2align 1
.Lbuffer:
  .short 0xc35a, 0, 0xa55a, 0, 0x5ac3
