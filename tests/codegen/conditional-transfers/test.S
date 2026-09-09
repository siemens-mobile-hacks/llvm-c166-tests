#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_transfer
#define RETURN ret
#else
.text
.c166_function huge, _test_transfer
#define RETURN rets
#endif
.globl _test_transfer
.type _test_transfer,@function
_test_transfer:
  push psw
  mov r10, sp
  mov r11, psw
  and r11, #0xffe0
  or r11, r13
  mov r4, #0
.macro dispatch id, cc
  cmp r12, #\id
  jmpr cc_eq, .L\cc
.endm
#define CONDITION(id, cc) dispatch id, cc
#include "conditions.inc"
#undef CONDITION
  jmpr cc_uc, .Ldone

.macro exercise id, cc
.L\cc:
  cmp r14, #0
  jmpr cc_eq, .Lrelative_\cc
  cmp r14, #1
  jmpr cc_eq, .Labsolute_\cc
  cmp r14, #2
  jmpr cc_eq, .Lindirect_\cc
  cmp r14, #3
  jmpr cc_eq, .Lcalla_\cc
  cmp r14, #4
  jmpr cc_eq, .Lcalli_\cc
  jmpr cc_uc, .Llong_\cc
.Lrelative_\cc:
  mov psw, r11
  jmpr \cc, .Ltaken_\cc
  jmpr cc_uc, .Ldone
.Labsolute_\cc:
  mov psw, r11
  jmpa \cc, .Ltaken_\cc
  jmpr cc_uc, .Ldone
.Lindirect_\cc:
  mov r1, #sof(.Ltaken_\cc)
  mov psw, r11
  jmpi \cc, [r1]
  jmpr cc_uc, .Ldone
.Ltaken_\cc:
  mov r5, psw
  bset r4.8
  mov psw, r5
  jmpr cc_uc, .Ldone
.Lcalla_\cc:
  mov psw, r11
  calla \cc, .Lcallee
  jmpr cc_uc, .Ldone
.Lcalli_\cc:
  mov r1, #sof(.Lcallee)
  mov psw, r11
  calli \cc, [r1]
  jmpr cc_uc, .Ldone
.Llong_\cc:
  mov psw, r11
  jmpr \cc, .Llong_taken_\cc
  jmpr cc_uc, .Ldone
  .space 256
.Llong_taken_\cc:
  mov r5, psw
  bset r4.8
  mov psw, r5
  jmpr cc_uc, .Ldone
.endm
#define CONDITION(id, cc) exercise id, cc
#include "conditions.inc"
#undef CONDITION

// Capture flags before MOV/ALU changes them. A taken near call must push
// exactly one return word below the saved entry SP.
.Lcallee:
  mov r5, psw
  mov r2, sp
  add r2, #2
  cmp r2, r10
  jmpr cc_eq, .Lcall_stack_ok
  bset r4.9
.Lcall_stack_ok:
  bset r4.8
  mov psw, r5
  ret
.Ldone:
  mov r5, psw
  and r5, #31
  or r4, r5
  mov r2, sp
  cmp r2, r10
  jmpr cc_eq, .Lstack_ok
  bset r4.9
.Lstack_ok:
  mov sp, r10
  pop psw
  RETURN
.size _test_transfer, .-_test_transfer
