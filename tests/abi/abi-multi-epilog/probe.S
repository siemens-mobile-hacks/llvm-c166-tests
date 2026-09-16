.text
.c166_function huge, _multi_epilog_pressure_probe
.globl _multi_epilog_pressure_probe
.type _multi_epilog_pressure_probe,@function
_multi_epilog_pressure_probe:
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

  calls seg(_multi_epilog_pressure), sof(_multi_epilog_pressure)
  push r4
  push r5
  mov r3, #0
  pop r11
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
  jmpr cc_eq, .Lall_ok
  mov r4, #0xc0de
  mov r5, #0xdead
  rets
.Lall_ok:
  mov r4, r10
  mov r5, r11
  rets
.size _multi_epilog_pressure_probe, .-_multi_epilog_pressure_probe
