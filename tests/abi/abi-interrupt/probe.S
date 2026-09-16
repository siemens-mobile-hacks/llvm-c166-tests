#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _run_interrupt_probe
#define RETURN ret
#else
.text
.c166_function huge, _run_interrupt_probe
#define RETURN rets
#endif

.macro fail code
  mov r4, #\code
  jmpa cc_uc, .Ldone
.endm

.macro check register, value, code
  cmp \register, \value
  jmpr cc_eq, .Lok\@
  fail \code
.Lok\@:
.endm

.globl _run_interrupt_probe
.type _run_interrupt_probe,@function
_run_interrupt_probe:
  push dpp0
  push dpp2
  push r1
  push r2
  push r3
  push r5
  push r6
  push r7
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov r4, csp
  push r0
  mov r9, sp
  mov r10, r0
  mov r6, cp
  mov r7, dpp3
  mov r8, dpp1
  mov r1, #0x1111
  mov r2, #0x2222
  mov r3, #0x3333
  mov r11, #0xbbbb
  mov r12, #0xcccc
  mov r13, #0xdddd
  mov r14, #0xeeee
  mov r15, #0xffff
  mov dpp0, #0x11
  mov dpp2, #0x22
  mov mdh, #0x1357
  mov mdl, #0x2468
  mov mdc, #5
  mov r5, psw
  trap #124

  push psw
  check r1, #0x1111, 0xe01a
  pop r1
  check r1, r5, 0xe001
  mov r1, csp
  check r1, r4, 0xe002
  check r2, #0x2222, 0xe003
  check r3, #0x3333, 0xe004
  check r11, #0xbbbb, 0xe00b
  check r12, #0xcccc, 0xe00c
  check r13, #0xdddd, 0xe00d
  check r14, #0xeeee, 0xe00e
  check r15, #0xffff, 0xe00f
  check r0, r10, 0xe010
  mov r1, sp
  check r1, r9, 0xe011
  mov r1, cp
  check r1, r6, 0xe012
  mov r1, dpp3
  check r1, r7, 0xe013
  mov r1, dpp1
  check r1, r8, 0xe014
  mov r1, dpp0
  check r1, #0x11, 0xe015
  mov r1, dpp2
  check r1, #0x22, 0xe016
  mov r1, mdc
  check r1, #5, 0xe017
  mov r1, mdh
  check r1, #0x1357, 0xe018
  mov r1, mdl
  check r1, #0x2468, 0xe019
  mov r4, #0

.Ldone:
  pop r0
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop r7
  pop r6
  pop r5
  pop r3
  pop r2
  pop r1
  pop dpp2
  pop dpp0
  RETURN
.size _run_interrupt_probe, .-_run_interrupt_probe
