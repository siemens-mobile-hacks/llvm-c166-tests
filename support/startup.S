.section .reset,"ax",@progbits
.globl _reset
_reset:
  jmps seg(_start), sof(_start)

.section .text.start,"ax",@progbits
.globl _start
.type _start,@function
#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.c166_function near, _start
#else
.c166_function huge, _start
#endif
_start:
  mov stkov, #0xfa0c
  mov sp, #0xfbfe
  mov stkun, #0xfbfe
  mov cp, #0xf600
  nop
  mov dpp0, #0
  mov dpp1, #1
  mov dpp2, #4
  mov dpp3, #3
#if __C166_MEMORY_MODEL__ == 4
  mov r0, #0xbffe
#else
  mov r0, #0x8000
#endif
  diswdt
  einit
  bset psw.11
#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
  calla cc_uc, cof(_c166_test_preinit)
  calla cc_uc, cof(_c166_crt_init)
  calla cc_uc, cof(_main)
  jmpa cc_uc, cof(_c166_test_halt)
#else
  calls seg(_c166_test_preinit), sof(_c166_test_preinit)
  calls seg(_c166_crt_init), sof(_c166_crt_init)
  calls seg(_main), sof(_main)
  jmps seg(_c166_test_halt), sof(_c166_test_halt)
#endif
.size _start, .-_start

.section .c166.halt,"ax",@progbits
.globl _c166_test_halt
_c166_test_halt:
  jmpa cc_uc, cof(_c166_test_halt)
