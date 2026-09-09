#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_boundaries
#define RETURN ret
#else
.text
.c166_function huge, _test_boundaries
#define RETURN rets
#endif

.globl _test_boundaries
.type _test_boundaries,@function
_test_boundaries:
  push psw
  mov r4, #0
.macro run_case id, target, flags
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  bfldl psw, #24, #\flags
  calls seg(\target), sof(\target)
  jmpa cc_uc, .Ldone
.Lnext\@:
.endm
  run_case 0, .Lcall_forward, 0
  run_case 1, .Lcall_backward, 0
  run_case 2, .Ljump_forward, 0
  run_case 3, .Ljump_backward, 0
  run_case 4, .Lbit_forward, 8
  run_case 5, .Lbit_backward, 8
  run_case 6, .Lrelax_uc, 0
  run_case 7, .Lrelax_net, 0
  run_case 8, .Lrelax_net, 24
  run_case 9, .Lrelax_bit, 8
  run_case 10, .Lrelax_bit, 0
.Ldone:
  pop psw
  RETURN
.size _test_boundaries, .-_test_boundaries

.section .edge.call.forward,"ax",@progbits
.Lcall_forward:
  callr .Lcall_forward_target
  rets
.section .wrap.call.forward,"ax",@progbits
.Lcall_forward_target:
  add r4, #1
  ret

.section .edge.call.backward,"ax",@progbits
.Lcall_backward:
  callr .Lcall_backward_target
  rets
.section .wrap.call.backward,"ax",@progbits
.Lcall_backward_target:
  add r4, #1
  ret

// Explicit relocations request fixed-size branches without relaxation padding.
.section .edge.jump.forward,"ax",@progbits
.Ljump_forward:
  .reloc .+1, R_C166_PC8, .Ljump_forward_target
  jmpr cc_uc, 0
.section .wrap.jump.forward,"ax",@progbits
  mov r4, #2
  rets
.Ljump_forward_target:
  mov r4, #1
  rets

.section .edge.jump.backward,"ax",@progbits
.Ljump_backward:
  .reloc .+1, R_C166_PC8, .Ljump_backward_target
  jmpr cc_uc, 0
  mov r4, #2
  rets
.section .wrap.jump.backward,"ax",@progbits
.Ljump_backward_target:
  mov r4, #1
  rets

.section .edge.bit.forward,"ax",@progbits
.Lbit_forward:
  .reloc .+2, R_C166_BIT_PC8, .Lbit_forward_target
  jb psw.3, 0
.section .wrap.bit.forward,"ax",@progbits
  mov r4, #2
  rets
.Lbit_forward_target:
  mov r4, #1
  rets

.section .edge.bit.backward,"ax",@progbits
.Lbit_backward:
  .reloc .+2, R_C166_BIT_PC8, .Lbit_backward_target
  jb psw.3, 0
  mov r4, #2
  rets
.section .wrap.bit.backward,"ax",@progbits
.Lbit_backward_target:
  mov r4, #1
  rets

.section .edge.relax.uc,"ax",@progbits
.Lrelax_uc:
  jmpr cc_uc, .Lrelax_uc_target
.section .wrap.relax.uc,"ax",@progbits
.Lrelax_uc_target:
  mov r4, #1
  rets

// A conditional slot ending at IP=ffff falls through to IP=0000 in the
// original CSP, while its taken JMPS changes CSP to the following segment.
.section .edge.relax.net,"ax",@progbits
.Lrelax_net:
  jmpr cc_net, .Lrelax_net_target
.section .fallthrough.net,"ax",@progbits
  mov r4, #2
  rets
.section .wrap.relax.net,"ax",@progbits
.Lrelax_net_target:
  mov r4, #1
  rets

.section .edge.relax.bit,"ax",@progbits
.Lrelax_bit:
  jb psw.3, .Lrelax_bit_target
.section .fallthrough.bit,"ax",@progbits
  mov r4, #2
  rets
.section .wrap.relax.bit,"ax",@progbits
.Lrelax_bit_target:
  mov r4, #1
  rets
