#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_trap
#define RETURN ret
#else
.text
.c166_function huge, _test_trap
#define RETURN rets
#endif

// R4 is a failure mask; R5 returns the PSW observed after RETI.
// The handler checks the frame before touching SP or executing a prologue.
.macro check lhs, rhs, error_bit
  cmp \lhs, \rhs
  jmpr cc_eq, .Lok\@
  bset r4.\error_bit
.Lok\@:
.endm

.globl _test_trap
.type _test_trap,@function
_test_trap:
  push psw
  push r6
  push r7
  push r8
  push r9
  push r10
  push r11
  mov r8, sp
  mov r10, r8
  sub r10, #6
  mov r4, #0
  mov r9, #seg(_test_trap)
  mov r6, csp
  check r6, r9, 10
  mov r11, #0

.macro invoke vector
  cmp r13, #\vector
  jmpr cc_ne, .Lnext\@
  mov r7, #sof(.Lreturned\@)
  mov psw, r12
  trap #\vector
.Lreturned\@:
  mov r5, psw
  jmpr cc_uc, .Lreturned
.Lnext\@:
.endm
  invoke 1
  invoke 63
  invoke 127
  bset r4.9
  mov r5, #0
.Lreturned:
  check r5, r12, 5
  mov r6, sp
  check r6, r8, 6
  mov r6, csp
  check r6, r9, 7
  check r11, #1, 8
  pop r11
  pop r10
  pop r9
  pop r8
  pop r7
  pop r6
  pop psw
  RETURN
.size _test_trap, .-_test_trap

.section .c166.interrupt.text,"ax",@progbits
.globl _trap_handler
.type _trap_handler,@function
_trap_handler:
  mov r5, psw
  check r5, r12, 0
  mov r6, sp
  check r6, r10, 1
  mov r5, [r6+]
  check r5, r7, 2
  mov r5, [r6+]
  check r5, r9, 3
  mov r5, [r6]
  check r5, r12, 4
  mov r11, #1
  // RETI must restore flags from the frame, not retain these handler flags.
  mov psw, #0
  reti
.size _trap_handler, .-_trap_handler
