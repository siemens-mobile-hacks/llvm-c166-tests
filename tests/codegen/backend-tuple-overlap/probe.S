.text

.macro probe name, target, setup, low, high
.globl \name
.type \name,@function
.c166_function huge, \name
\name:
  \setup
  calls seg(\target), sof(\target)
  mov r4, \low
  mov r5, \high
  rets
.size \name, .-\name
.endm

.macro setup_copy_up
  mov r1, r12
  mov r2, r13
.endm
.macro setup_scalar
  mov r2, r12
.endm
.macro setup_copy_down
  mov r2, r12
  mov r3, r13
.endm

probe _copy_pair_up_probe, _copy_pair_up, setup_copy_up, r2, r3
probe _zext_pair_high_overlap_probe, _zext_pair_high_overlap, setup_scalar, r1, r2
probe _sext_pair_high_overlap_probe, _sext_pair_high_overlap, setup_scalar, r1, r2
probe _copy_pair_down_probe, _copy_pair_down, setup_copy_down, r1, r2
