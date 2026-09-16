#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_esfr
#define RETURN ret
#else
.text
.c166_function huge, _test_esfr
#define RETURN rets
#endif

// C167: CC1IC=FF7A and T7IC=F17A share bitoff BD. Bits0..5 are priority;
// IE/IR are bits6/7. No timer or interrupt is enabled by this test.
.equ CONTROL, 0xbd
.globl _test_esfr
.type _test_esfr,@function
_test_esfr:
  push psw
  push dpp3
  push r9
  push r10
  mov psw, #0
  mov dpp3, #3
  mov r9, 0xff7a
  mov r10, 0xf17a
  mov r1, #0
  mov 0xff7a, r1
  mov 0xf17a, r1

.macro exercise mode, count, extension
  cmp r12, #(4*\mode+\count-1)
  jmpr cc_ne, .Lnext\@
  .if \mode == 6
    mov r1, #1
    mov 0xf17a, r1
    mov r4, #0
    mov r5, #0
  .elseif (\mode >= 12) && (\mode < 17)
    mov r1, #1
    mov 0xf17a, r1
    mov r1, #42
    mov 0xff7a, r1
    mov r3, sp
  .endif
  .if \extension == 2 || \extension == 6
    mov r2, #17
  .elseif \extension == 4 || \extension == 8
    mov r2, #1
  .endif
  .if \mode >= 17
    mov psw, r13
  .endif
  .if \extension == 0
    extr #\count
  .elseif \extension == 1
    extpr #17, #\count
  .elseif \extension == 2
    extpr r2, #\count
  .elseif \extension == 3
    extsr #1, #\count
  .elseif \extension == 4
    extsr r2, #\count
  .elseif \extension == 5
    extp #17, #\count
  .elseif \extension == 6
    extp r2, #\count
  .elseif \extension == 7
    exts #1, #\count
  .elseif \extension == 8
    exts r2, #\count
  .else
    atomic #\count
  .endif
  .set .Lstep\@, 0
  .rept \count
    .if \mode >= 17
      nop
    .elseif \mode >= 12
      push sfr(CONTROL)
    .elseif \mode >= 7
      mov sfr(CONTROL), #(1 << .Lstep\@)
    .elseif \mode < 5
      bfldl CONTROL, #63, #(1 << .Lstep\@)
    .elseif \mode == 5
      bset CONTROL . .Lstep\@
    .else
      bmov r4.0, CONTROL.0
    .endif
    .set .Lstep\@, .Lstep\@ + 1
  .endr
  .if \mode >= 17
    mov r4, psw
    and r4, #31
    mov r5, #0
    jmpr cc_uc, .Lrestore
  .elseif \mode >= 12
    push sfr(CONTROL)
    pop r5
    .rept \count
      pop r4
      cmp r4, #1
      jmpr cc_eq, 1f
      or r5, #0x8000
1:
    .endr
    cmp r3, sp
    jmpr cc_eq, 1f
    or r5, #0x8000
1:
    mov sp, r3
    mov r1, 0xf17a
    cmp r1, #1
    jmpr cc_ne, .Lread_bad\@
    mov r1, 0xff7a
    cmp r1, #42
    jmpr cc_eq, .Lrestore
.Lread_bad\@:
    or r5, #0x8000
    jmpr cc_uc, .Lrestore
  .elseif \mode >= 7
    mov sfr(CONTROL), #42
  .elseif \mode < 5
    bfldl CONTROL, #63, #42
  .elseif \mode == 5
    bset CONTROL.5
  .else
    bmov r5.0, CONTROL.0
    jmpr cc_uc, .Lrestore
  .endif
  mov r4, 0xf17a
  mov r5, 0xff7a
  jmpr cc_uc, .Lrestore
.Lnext\@:
.endm
  .irp extension, 0, 1, 2, 3, 4
    .irp count, 1, 2, 3, 4
      exercise \extension, \count, \extension
      exercise (7+\extension), \count, \extension
      exercise (12+\extension), \count, \extension
    .endr
  .endr
  .irp mode, 5, 6
    .irp count, 1, 2, 3, 4
      exercise \mode, \count, 0
    .endr
  .endr
  .irp extension, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    .irp count, 1, 2, 3, 4
      exercise (17+\extension), \count, \extension
    .endr
  .endr
  mov r4, #0xffff
  mov r5, #0xffff
.Lrestore:
  mov 0xf17a, r10
  mov 0xff7a, r9
  pop r10
  pop r9
  pop dpp3
  pop psw
  RETURN
.size _test_esfr, .-_test_esfr
