#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _c166_saved_probe
#define CALL_ENTRY calla cc_uc, cof(_c166_saved_entry)
#define RETURN ret
#else
.text
.c166_function huge, _c166_saved_probe
#define CALL_ENTRY calls seg(_c166_saved_entry), sof(_c166_saved_entry)
#define RETURN rets
#endif

.globl _c166_saved_probe
.type _c166_saved_probe,@function
_c166_saved_probe:
  push r6
  push r7
  push r8
  push r9
  mov r6, #0x6006
  mov r7, #0x7007
  mov r8, #0x8008
  mov r9, #0x9009
  CALL_ENTRY
  mov r5, r4
  mov r4, #0xdead
  cmp r6, #0x6006
  jmpr cc_ne, .Ldone
  cmp r7, #0x7007
  jmpr cc_ne, .Ldone
  cmp r8, #0x8008
  jmpr cc_ne, .Ldone
  cmp r9, #0x9009
  jmpr cc_ne, .Ldone
  mov r4, r5
.Ldone:
  pop r9
  pop r8
  pop r7
  pop r6
  RETURN
.size _c166_saved_probe, .-_c166_saved_probe
