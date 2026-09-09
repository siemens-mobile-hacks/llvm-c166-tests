#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_near_calls
#define RETURN ret
#else
.text
.c166_function huge, _test_near_calls
#define RETURN rets
#endif

// R4 is a failure mask. R10 is SP after saving the caller's PSW.
// The host supplies the word in R12, input flags in R13, and expected
// PCALL/RETP flags in R14. Only caller-clobbered registers are used.
.macro check_sp depth
  mov r1, sp
  add r1, #\depth
  cmp r1, r10
  jmpr cc_eq, .Lsp_ok\@
  or r4, #2
.Lsp_ok\@:
.endm
.macro check_return label
  mov r1, sp
  mov r2, [r1]
  cmp r2, #sof(\label)
  jmpr cc_eq, .Lip_ok\@
  or r4, #4
.Lip_ok\@:
.endm

.globl _test_near_calls
.type _test_near_calls,@function
_test_near_calls:
  push psw
  mov r10, sp
  mov r11, psw
  and r11, #0xffe0
  or r11, r13
  mov r4, #0
  jmpa cc_uc, .Lstart

.Lrelative:
  mov r5, psw
  check_sp 2
  check_return .Lafter_relative
  callr .Lnested
.Lafter_nested:
  check_sp 2
  mov psw, r5
  ret
.Lnested:
  mov r15, psw
  check_sp 4
  check_return .Lafter_nested
  mov psw, r15
  ret

.Lpcall:
  mov r5, psw
  and r5, #31
  cmp r5, r14
  jmpr cc_eq, .Lpcall_flags_ok
  or r4, #16
.Lpcall_flags_ok:
  check_sp 4
  check_return .Lafter_pcall
  mov r1, sp
  add r1, #2
  mov r2, [r1]
  cmp r2, r12
  jmpr cc_eq, .Lsaved_word_ok
  or r4, #8
.Lsaved_word_ok:
  mov r3, #0x1234
  // Restore input C/V; RETP must derive N/Z/E from the popped word.
  mov psw, r11
  retp r5

.Lstart:
  mov psw, r11
  callr .Lrelative
.Lafter_relative:
  mov r15, psw
  and r15, #31
  cmp r15, r13
  jmpr cc_eq, .Lcallr_flags_ok
  or r4, #1
.Lcallr_flags_ok:
  check_sp 0
  mov r3, r12
  mov psw, r11
  pcall r3, .Lpcall
.Lafter_pcall:
  mov r15, psw
  cmp r5, r12
  jmpr cc_eq, .Lrestored_word_ok
  or r4, #32
.Lrestored_word_ok:
  and r15, #31
  cmp r15, r14
  jmpr cc_eq, .Lretp_flags_ok
  or r4, #64
.Lretp_flags_ok:
  check_sp 0
  mov sp, r10
  pop psw
  RETURN
.size _test_near_calls, .-_test_near_calls
