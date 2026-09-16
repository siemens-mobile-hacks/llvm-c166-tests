#if __C166_MEMORY_MODEL__ == 2
.section .c166.near.text,"ax",@progbits
#define FUNCTION(name) .c166_function near, name
#define CALL(name) calla cc_uc, name
#define RETURN ret
#else
.text
#define FUNCTION(name) .c166_function huge, name
#define CALL(name) calls seg(name), sof(name)
#define RETURN rets
#endif

FUNCTION(_tuple_reload_low_probe)
.globl _tuple_reload_low_probe
.type _tuple_reload_low_probe,@function
_tuple_reload_low_probe:
  mov r2, #0
  CALL(.Ltuple_reload_probe)
  RETURN
.size _tuple_reload_low_probe, .-_tuple_reload_low_probe

FUNCTION(_tuple_reload_high_probe)
.globl _tuple_reload_high_probe
.type _tuple_reload_high_probe,@function
_tuple_reload_high_probe:
  mov r2, #1
  CALL(.Ltuple_reload_probe)
  RETURN
.size _tuple_reload_high_probe, .-_tuple_reload_high_probe

FUNCTION(.Ltuple_reload_probe)
.Ltuple_reload_probe:
  mov r1, sp
  push r1
  push r0
  push r6
  push r7
  push r8
  push r9
  mov r1, dpp0
  push r1
  mov r1, dpp1
  push r1
  mov r1, dpp2
  push r1
  mov r1, dpp3
  push r1

  cmp r2, #0
  jmpr cc_eq, .Lcall_low
  CALL(_tuple_reload_high)
  jmpr cc_uc, .Lcall_done
.Lcall_low:
  CALL(_tuple_reload_low)
.Lcall_done:
  push r4
  mov r3, #0
  pop r10

  pop r1
  mov r2, dpp3
  cmp r1, r2
  jmpr cc_eq, .Ldpp3_ok
  mov r3, #1
.Ldpp3_ok:
  pop r1
  mov r2, dpp2
  cmp r1, r2
  jmpr cc_eq, .Ldpp2_ok
  mov r3, #1
.Ldpp2_ok:
  pop r1
  mov r2, dpp1
  cmp r1, r2
  jmpr cc_eq, .Ldpp1_ok
  mov r3, #1
.Ldpp1_ok:
  pop r1
  mov r2, dpp0
  cmp r1, r2
  jmpr cc_eq, .Ldpp0_ok
  mov r3, #1
.Ldpp0_ok:

  pop r1
  cmp r1, r9
  jmpr cc_eq, .Lr9_ok
  mov r3, #1
.Lr9_ok:
  mov r9, r1
  pop r1
  cmp r1, r8
  jmpr cc_eq, .Lr8_ok
  mov r3, #1
.Lr8_ok:
  mov r8, r1
  pop r1
  cmp r1, r7
  jmpr cc_eq, .Lr7_ok
  mov r3, #1
.Lr7_ok:
  mov r7, r1
  pop r1
  cmp r1, r6
  jmpr cc_eq, .Lr6_ok
  mov r3, #1
.Lr6_ok:
  mov r6, r1

  pop r1
  cmp r1, r0
  jmpr cc_eq, .Lr0_ok
  mov r3, #1
.Lr0_ok:
  mov r0, r1
  pop r1
  mov r2, sp
  cmp r1, r2
  jmpr cc_eq, .Lsp_ok
  mov r3, #1
.Lsp_ok:

  cmp r3, #0
  jmpr cc_eq, .Lstate_ok
  mov r4, #0xc0de
  RETURN
.Lstate_ok:
  mov r4, r10
  RETURN
