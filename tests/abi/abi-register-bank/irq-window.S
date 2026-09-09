#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _irq_window
#define RETURN ret
#else
.text
.c166_function huge, _irq_window
#define RETURN rets
#endif
.globl _irq_window
.type _irq_window,@function
_irq_window:
  push psw
  mov psw, #0
  push sfr(0xb1)
  push mdh
  push r6
  mov mdh, #0
  mov sfr(0xb1), #0xf8
  mov r1, #0xff10
  mov r2, #0x800
  mov r3, #0xfe0c
  mov r4, #1
  mov r5, #2
  mov r6, #3

// Page 3 and segment 0 preserve these physical addresses. Indirect accesses
// also avoid short-SFR redirection under EXTR and the EXT*R forms.
.macro protection mode, count, reg=r13
  .if \mode == 1
    atomic #\count
  .elseif \mode == 2
    extr #\count
  .elseif \mode == 3
    extp #3, #\count
  .elseif \mode == 4
    mov \reg, #3
    extp \reg, #\count
  .elseif \mode == 5
    exts #0, #\count
  .elseif \mode == 6
    mov \reg, #0
    exts \reg, #\count
  .elseif \mode == 7
    extpr #3, #\count
  .elseif \mode == 8
    mov \reg, #3
    extpr \reg, #\count
  .elseif \mode == 9
    extsr #0, #\count
  .elseif \mode == 10
    mov \reg, #0
    extsr \reg, #\count
  .endif
.endm
.macro window mode, count, id
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  protection \mode, \count
  mov [r1], r2
  .if \count >= 2
  mov [r3], r4
  .endif
  .if \count >= 3
  mov [r3], r5
  .endif
  .if \count >= 4
  mov [r3], r6
  .endif
  .rept 8
    nop
  .endr
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
window 0, 4, 0
.irp count, 1,2,3,4
.irp mode, 1,2,3,4,5,6,7,8,9,10
  window \mode, \count, (\mode + 16 * (\count - 1))
.endr
.endr
.Ldone:
  bclr psw.11
  pop r6
  pop mdh
  pop sfr(0xb1)
  pop psw
  RETURN
.size _irq_window, .-_irq_window

#if __C166_MEMORY_MODEL__ == 2
.c166_function near, _pec_transfer
#else
.c166_function huge, _pec_transfer
#endif
.globl _pec_transfer
.type _pec_transfer,@function
_pec_transfer:
  push psw
  mov psw, #0
  push sfr(0xb1)
  push sfr(0x60)
  push mdh
  mov r1, 0xfce0
  push r1
  mov r1, 0xfce2
  push r1
  sub r0, #4
  mov r1, #0x5a3c
  mov [r0], r1
  mov r1, #0xffff
  mov [r0 + #2], r1
  // The harness user stack uses DPP1=1, within PEC's segment-zero range.
  mov 0xfce0, r0
  mov r1, r0
  add r1, #2
  mov 0xfce2, r1
  mov sfr(0x60), #2
  mov sfr(0xb1), #0xf8
  cmp r12, #0
  jmpr cc_eq, .Lpec_baseline
  mov mdh, #0
  mov r15, #0xfe0c
  mov 0xfce0, r15
  mov r13, #0xff10
  mov r5, #0x800
  mov r1, #1
  mov r2, #2
  mov r3, #3
.macro pec_window mode
  cmp r12, #(\mode + 1)
  jmpr cc_ne, .Lpec_next\@
  protection \mode, 4, r14
  mov [r13], r5
  mov [r15], r1
  mov [r15], r2
  mov [r15], r3
  jmpr cc_uc, .Lpec_wait
.Lpec_next\@:
.endm
.irp mode, 0,1,2,3,4,5,6,7,8,9,10
  pec_window \mode
.endr
.Lpec_baseline:
  mov psw, #0x800
.Lpec_wait:
  .rept 8
    nop
  .endr
  bclr psw.11
  mov r4, #1
  mov r1, [r0 + #2]
  cmp r12, #0
  jmpr cc_ne, .Lpec_check_window
  cmp r1, #0x5a3c
  jmpr cc_ne, .Lpec_done
  jmpr cc_uc, .Lpec_check_count
.Lpec_check_window:
  cmp r12, #1
  jmpr cc_ne, .Lpec_check_protected
  cmp r1, #3
  jmpr cc_uge, .Lpec_done
  jmpr cc_uc, .Lpec_check_count
.Lpec_check_protected:
  cmp r1, #3
  jmpr cc_ne, .Lpec_done
.Lpec_check_count:
  mov r4, #2
  mov r1, sfr(0x60)
  cmp r1, #1
  jmpr cc_ne, .Lpec_done
  mov r4, #3
  mov r1, sfr(0xb1)
  and r1, #0x80
  jmpr cc_ne, .Lpec_done
  mov r4, #0
.Lpec_done:
  add r0, #4
  pop r1
  mov 0xfce2, r1
  pop r1
  mov 0xfce0, r1
  pop mdh
  pop sfr(0x60)
  pop sfr(0xb1)
  pop psw
  RETURN
.size _pec_transfer, .-_pec_transfer
