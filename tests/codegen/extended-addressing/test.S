#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_extended_addressing
#define RETURN ret
#else
.text
.c166_function huge, _test_extended_addressing
#define RETURN rets
#endif
.globl _test_extended_addressing
.type _test_extended_addressing,@function
_test_extended_addressing:
  push psw
  push dpp1
  push r6
  push r7
  push r10
  push r11
  mov dpp1, #pag(.Ldefault)
  mov r1, #0x4000

// Read at the last active instruction and once after expiry. EXTP ignores
// address bits 15:14; EXTS retains them. Indirect GPR accesses also work in
// the register-space variants without touching device-specific ESFRs.
.macro write_value source, direct
  .if \direct
    mov 0x4000, \source
  .else
    mov [r1], \source
  .endif
.endm
.macro exercise id, insn, operand, count, register_value, direct=0, store=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov r2, #\register_value
  .if \store
    // Independent linear access saves and later checks the overridden target.
    .if (\id & 8)
      mov r7, #sof(.Lsegment)
    .else
      mov r7, #sof(.Lpage)
    .endif
    exts #seg(.Lpage), #1
    mov r10, [r7]
    mov r11, [r1]
    mov r3, #0x1111
    mov r4, #0x2222
    mov r5, #0x3333
    mov r6, #0x4444
    mov r13, #0xa53c
  .endif
  .set .Lstep\@, 0
  \insn \operand, #\count
  .rept \count
  .if \store
    .if .Lstep\@ == 0
      write_value r3, \direct
    .elseif .Lstep\@ == 1
      write_value r4, \direct
    .elseif .Lstep\@ == 2
      write_value r5, \direct
    .else
      write_value r6, \direct
    .endif
  .else
  .if \direct
    mov r4, 0x4000
  .else
    mov r4, [r1]
  .endif
  .endif
  .set .Lstep\@, .Lstep\@ + 1
  .endr
  .if \store
    // This must be the first instruction after the EXT sequence expires.
    mov [r1], r13
    exts #seg(.Lpage), #1
    mov r4, [r7]
    mov r5, [r1]
    exts #seg(.Lpage), #1
    mov [r7], r10
    mov [r1], r11
  .else
  mov r5, [r1]
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  exercise 0, extp, #pag(.Lpage), 1, 260
  exercise 1, extp, #pag(.Lpage), 2, 260
  exercise 2, extp, #pag(.Lpage), 3, 260
  exercise 3, extp, #pag(.Lpage), 4, 260
  exercise 4, extp, r2, 1, 260
  exercise 5, extp, r2, 2, 260
  exercise 6, extp, r2, 3, 260
  exercise 7, extp, r2, 4, 260
  exercise 8, exts, #seg(.Lsegment), 1, 65
  exercise 9, exts, #seg(.Lsegment), 2, 65
  exercise 10, exts, #seg(.Lsegment), 3, 65
  exercise 11, exts, #seg(.Lsegment), 4, 65
  exercise 12, exts, r2, 1, 65
  exercise 13, exts, r2, 2, 65
  exercise 14, exts, r2, 3, 65
  exercise 15, exts, r2, 4, 65
  exercise 16, extpr, #pag(.Lpage), 1, 260
  exercise 17, extpr, #pag(.Lpage), 2, 260
  exercise 18, extpr, #pag(.Lpage), 3, 260
  exercise 19, extpr, #pag(.Lpage), 4, 260
  exercise 20, extpr, r2, 1, 260
  exercise 21, extpr, r2, 2, 260
  exercise 22, extpr, r2, 3, 260
  exercise 23, extpr, r2, 4, 260
  exercise 24, extsr, #seg(.Lsegment), 1, 65
  exercise 25, extsr, #seg(.Lsegment), 2, 65
  exercise 26, extsr, #seg(.Lsegment), 3, 65
  exercise 27, extsr, #seg(.Lsegment), 4, 65
  exercise 28, extsr, r2, 1, 65
  exercise 29, extsr, r2, 2, 65
  exercise 30, extsr, r2, 3, 65
  exercise 31, extsr, r2, 4, 65
  exercise 32, extp, #pag(.Lpage), 1, 260, 1
  exercise 33, extp, #pag(.Lpage), 2, 260, 1
  exercise 34, extp, #pag(.Lpage), 3, 260, 1
  exercise 35, extp, #pag(.Lpage), 4, 260, 1
  exercise 36, extp, r2, 1, 260, 1
  exercise 37, extp, r2, 2, 260, 1
  exercise 38, extp, r2, 3, 260, 1
  exercise 39, extp, r2, 4, 260, 1
  exercise 40, exts, #seg(.Lsegment), 1, 65, 1
  exercise 41, exts, #seg(.Lsegment), 2, 65, 1
  exercise 42, exts, #seg(.Lsegment), 3, 65, 1
  exercise 43, exts, #seg(.Lsegment), 4, 65, 1
  exercise 44, exts, r2, 1, 65, 1
  exercise 45, exts, r2, 2, 65, 1
  exercise 46, exts, r2, 3, 65, 1
  exercise 47, exts, r2, 4, 65, 1
  exercise 48, extpr, #pag(.Lpage), 1, 260, 1
  exercise 49, extpr, #pag(.Lpage), 2, 260, 1
  exercise 50, extpr, #pag(.Lpage), 3, 260, 1
  exercise 51, extpr, #pag(.Lpage), 4, 260, 1
  exercise 52, extpr, r2, 1, 260, 1
  exercise 53, extpr, r2, 2, 260, 1
  exercise 54, extpr, r2, 3, 260, 1
  exercise 55, extpr, r2, 4, 260, 1
  exercise 56, extsr, #seg(.Lsegment), 1, 65, 1
  exercise 57, extsr, #seg(.Lsegment), 2, 65, 1
  exercise 58, extsr, #seg(.Lsegment), 3, 65, 1
  exercise 59, extsr, #seg(.Lsegment), 4, 65, 1
  exercise 60, extsr, r2, 1, 65, 1
  exercise 61, extsr, r2, 2, 65, 1
  exercise 62, extsr, r2, 3, 65, 1
  exercise 63, extsr, r2, 4, 65, 1
  // The same count/source/addressing matrix for writes, without copied bodies.
  .irp direct, 0, 1
    .irp count, 1, 2, 3, 4
      exercise (63+32*\direct+\count), extp, #pag(.Lpage), \count, 260, \direct, 1
      exercise (67+32*\direct+\count), extp, r2, \count, 260, \direct, 1
      exercise (71+32*\direct+\count), exts, #seg(.Lsegment), \count, 65, \direct, 1
      exercise (75+32*\direct+\count), exts, r2, \count, 65, \direct, 1
      exercise (79+32*\direct+\count), extpr, #pag(.Lpage), \count, 260, \direct, 1
      exercise (83+32*\direct+\count), extpr, r2, \count, 260, \direct, 1
      exercise (87+32*\direct+\count), extsr, #seg(.Lsegment), \count, 65, \direct, 1
      exercise (91+32*\direct+\count), extsr, r2, \count, 65, \direct, 1
    .endr
  .endr
// A nested EXT reloads the single sequence counter; expiry must not resume
// the outer page/segment override. The old matrix checks non-nested accesses.
.macro nested id, outer, outer_value, inner, inner_value, outer_count, inner_count
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  \outer \outer_value, #\outer_count
  \inner \inner_value, #\inner_count
  .rept \inner_count - 1
    nop
  .endr
  mov r4, [r1]
  mov r5, [r1]
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  .irp outer_count, 1, 2, 3, 4
    .irp inner_count, 1, 2, 3, 4
      nested (123+4*\outer_count+\inner_count), extp, #pag(.Lpage), extp, #pag(.Lpage), \outer_count, \inner_count
      nested (139+4*\outer_count+\inner_count), extp, #pag(.Lpage), exts, #seg(.Lsegment), \outer_count, \inner_count
      nested (155+4*\outer_count+\inner_count), exts, #seg(.Lsegment), extp, #pag(.Lpage), \outer_count, \inner_count
      nested (171+4*\outer_count+\inner_count), exts, #seg(.Lsegment), exts, #seg(.Lsegment), \outer_count, \inner_count
    .endr
  .endr
  mov r4, #0
  mov r5, #0
.Ldone:
  pop r11
  pop r10
  pop r7
  pop r6
  pop dpp1
  pop psw
  RETURN
.size _test_extended_addressing, .-_test_extended_addressing

.section .ext.page,"aw",@progbits
.Lpage:
  .short 0x1357
.section .ext.segment,"aw",@progbits
.Lsegment:
  .short 0x2468
.section .ext.default,"aw",@progbits
.Ldefault:
  .short 0x5aa5
