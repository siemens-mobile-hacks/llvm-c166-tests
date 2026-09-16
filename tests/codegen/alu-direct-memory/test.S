#if __C166_MEMORY_MODEL__ == 2 || __C166_MEMORY_MODEL__ == 4
.section .c166.near.text,"ax",@progbits
.c166_function near, _test_alu_direct_memory
#define RETURN ret
#else
.text
.c166_function huge, _test_alu_direct_memory
#define RETURN rets
#endif
.globl _test_alu_direct_memory
.type _test_alu_direct_memory,@function
_test_alu_direct_memory:
  push psw
  push mdc
  push mdh
  push mdl
  push dpp1
  mov dpp1, #pag(.Loperand)
  mov 0x4002, r14
  mov r3, r14
  mov r1, r14
  mov r4, r13

// Inputs: operation, lhs, rhs, initial flags. Return result and captured flags.
.macro operation id, mnemonic, memory_destination=0, byte=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  .if \memory_destination
    mov 0x4002, r13
    mov psw, r15
    .if \byte
      \mnemonic 0x4002, rl1
    .else
      \mnemonic 0x4002, r14
    .endif
    mov r5, psw
    mov r4, 0x4002
  .else
    .if \byte
      mov r2, r13
      mov psw, r15
      \mnemonic rl2, 0x4002
      mov r5, psw
      mov r4, r2
    .else
      mov psw, r15
      \mnemonic r4, 0x4002
      mov r5, psw
    .endif
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  operation 0, add
  operation 1, addc
  operation 2, sub
  operation 3, subc
  operation 4, cmp
  operation 5, xor
  operation 6, and
  operation 7, or
  operation 8, add, 1
  operation 9, addc, 1
  operation 10, sub, 1
  operation 11, subc, 1
  operation 12, xor, 1
  operation 13, and, 1
  operation 14, or, 1
  operation 15, addb, 0, 1
  operation 16, addcb, 0, 1
  operation 17, subb, 0, 1
  operation 18, subcb, 0, 1
  operation 19, cmpb, 0, 1
  operation 20, xorb, 0, 1
  operation 21, andb, 0, 1
  operation 22, orb, 0, 1
  operation 23, addb, 1, 1
  operation 24, addcb, 1, 1
  operation 25, subb, 1, 1
  operation 26, subcb, 1, 1
  operation 27, xorb, 1, 1
  operation 28, andb, 1, 1
  operation 29, orb, 1, 1

// High-byte destination and odd memory address. Verify the complete source
// word and pointer before restoring scratch registers for the common checks.
.macro byte_indirect id, mnemonic, increment=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov r2, r13
  rol r2, #8
  mov r3, r14
  rol r3, #8
  mov 0x4002, r3
  mov r1, #0x4003
  mov psw, r15
  .if \increment
    \mnemonic rh2, [r1+]
  .else
    \mnemonic rh2, [r1]
  .endif
  mov r5, psw
  and r5, #31
  mov r4, r2
  ror r4, #8
  cmp r1, #(0x4003 + \increment)
  jmpr cc_ne, .Lcorrupted
  mov r2, 0x4002
  cmp r2, r3
  jmpr cc_ne, .Lcorrupted
  mov r3, r14
  mov r1, r14
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  byte_indirect 30, addcb
  byte_indirect 31, subcb
  byte_indirect 32, addcb, 1
  byte_indirect 33, subcb, 1
  byte_indirect 71, addb
  byte_indirect 72, subb
  byte_indirect 73, cmpb
  byte_indirect 74, xorb
  byte_indirect 75, andb
  byte_indirect 76, orb
  byte_indirect 77, addb, 1
  byte_indirect 78, subb, 1
  byte_indirect 79, cmpb, 1
  byte_indirect 80, xorb, 1
  byte_indirect 81, andb, 1
  byte_indirect 82, orb, 1

.macro carry_register id, mnemonic
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov r2, r13
  rol r2, #8
  rol r1, #8
  mov psw, r15
  \mnemonic rh2, rh1
  mov r5, psw
  mov r4, r2
  ror r4, #8
  ror r1, #8
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  carry_register 34, addcb
  carry_register 35, subcb

.macro carry_constant mnemonic, value
  cmp r14, #(0x5a00 + \value)
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  \mnemonic rh2, #\value
  mov r5, psw
  mov r4, r2
  ror r4, #8
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
.macro carry_immediate id, mnemonic, full=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov r2, r13
  rol r2, #8
  .if \full
    .irp value, 8, 16, 127, 128, 254, 255, 90, 64
      carry_constant \mnemonic, \value
    .endr
  .else
    .irp value, 0, 1, 2, 3, 4, 5, 6, 7
      carry_constant \mnemonic, \value
    .endr
  .endif
.Lnext\@:
.endm
  carry_immediate 36, addcb
  carry_immediate 37, subcb
  carry_immediate 38, addcb, 1
  carry_immediate 39, subcb, 1

.macro sfr_constant mnemonic, value, byte=0
  cmp r14, #(\value + \byte * 0x5a00)
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  \mnemonic mdl, #\value
  mov r5, psw
  mov r4, mdl
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
.macro sfr_immediate id, mnemonic, byte=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov mdl, r13
  .if \byte
    .irp value, 0, 1, 2, 127, 128, 254, 255, 90
      sfr_constant \mnemonic, \value, 1
    .endr
  .else
    .irp value, 0, 1, 7, 32767, 32768, 65534, 65535, 42330
      sfr_constant \mnemonic, \value
    .endr
  .endif
.Lnext\@:
.endm
  sfr_immediate 40, add
  sfr_immediate 41, addc
  sfr_immediate 42, sub
  sfr_immediate 43, subc
  sfr_immediate 44, cmp
  sfr_immediate 45, xor
  sfr_immediate 46, and
  sfr_immediate 47, or
  sfr_immediate 83, addb, 1
  sfr_immediate 84, addcb, 1
  sfr_immediate 85, subb, 1
  sfr_immediate 86, subcb, 1
  sfr_immediate 87, cmpb, 1
  sfr_immediate 88, xorb, 1
  sfr_immediate 89, andb, 1
  sfr_immediate 90, orb, 1

.macro sfr_memory id, mnemonic, memory_destination=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  .if \memory_destination
    mov mdl, r14
    mov 0x4002, r13
    mov psw, r15
    \mnemonic 0x4002, mdl
    mov r5, psw
    and r5, #31
    mov r4, 0x4002
    mov r2, mdl
    cmp r2, r14
    jmpr cc_ne, .Lcorrupted
  .else
    mov mdl, r13
    mov psw, r15
    \mnemonic mdl, 0x4002
    mov r5, psw
    mov r4, mdl
  .endif
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  sfr_memory 48, add
  sfr_memory 49, addc
  sfr_memory 50, sub
  sfr_memory 51, subc
  sfr_memory 52, cmp
  sfr_memory 53, xor
  sfr_memory 54, and
  sfr_memory 55, or
  sfr_memory 56, add, 1
  sfr_memory 57, addc, 1
  sfr_memory 58, sub, 1
  sfr_memory 59, subc, 1
  sfr_memory 60, xor, 1
  sfr_memory 61, and, 1
  sfr_memory 62, or, 1
  sfr_memory 91, addb
  sfr_memory 92, addcb
  sfr_memory 93, subb
  sfr_memory 94, subcb
  sfr_memory 95, cmpb
  sfr_memory 96, xorb
  sfr_memory 97, andb
  sfr_memory 98, orb
  sfr_memory 99, addb, 1
  sfr_memory 100, addcb, 1
  sfr_memory 101, subb, 1
  sfr_memory 102, subcb, 1
  sfr_memory 103, xorb, 1
  sfr_memory 104, andb, 1
  sfr_memory 105, orb, 1

.macro sfr_register id, mnemonic
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov mdl, r13
  mov mdh, r14
  mov psw, r15
  \mnemonic mdl, mdh
  mov r5, psw
  and r5, #31
  mov r4, mdl
  mov r2, mdh
  cmp r2, r14
  jmpr cc_ne, .Lcorrupted
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
  sfr_register 63, add
  sfr_register 64, addc
  sfr_register 65, sub
  sfr_register 66, subc
  sfr_register 67, cmp
  sfr_register 68, xor
  sfr_register 69, and
  sfr_register 70, or
  sfr_register 106, addb
  sfr_register 107, addcb
  sfr_register 108, subb
  sfr_register 109, subcb
  sfr_register 110, cmpb
  sfr_register 111, xorb
  sfr_register 112, andb
  sfr_register 113, orb

.macro loop_constant mnemonic, value
  cmp r14, #\value
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  \mnemonic r4, #\value
  mov r5, psw
  jmpr cc_uc, .Ldone
.Lnext\@:
.endm
.macro loop_compare id, mnemonic, form
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  .if \form == 0
    .irp value, 0, 1, 2, 7, 8, 9, 14, 15
      loop_constant \mnemonic, \value
    .endr
  .elseif \form == 1
    .irp value, 16, 17, 255, 256, 32767, 32768, 65534, 65535
      loop_constant \mnemonic, \value
    .endr
  .else
    mov psw, r15
    \mnemonic r4, 0x4002
    mov r5, psw
    jmpr cc_uc, .Ldone
  .endif
.Lnext\@:
.endm
  loop_compare 114, cmpd1, 0
  loop_compare 115, cmpd2, 0
  loop_compare 116, cmpi1, 0
  loop_compare 117, cmpi2, 0
  loop_compare 118, cmpd1, 1
  loop_compare 119, cmpd2, 1
  loop_compare 120, cmpi1, 1
  loop_compare 121, cmpi2, 1
  loop_compare 122, cmpd1, 2
  loop_compare 123, cmpd2, 2
  loop_compare 124, cmpi1, 2
  loop_compare 125, cmpi2, 2

.macro word_indirect id, mnemonic, increment=0
  cmp r12, #\id
  jmpr cc_ne, .Lnext\@
  mov psw, r15
  .if \increment
    \mnemonic r4, [r1+]
  .else
    \mnemonic r4, [r1]
  .endif
  jmpr cc_uc, .Lword_done
.Lnext\@:
.endm
  mov r1, #0x4002
  word_indirect 126, add
  word_indirect 127, addc
  word_indirect 128, sub
  word_indirect 129, subc
  word_indirect 130, cmp
  word_indirect 131, xor
  word_indirect 132, and
  word_indirect 133, or
  word_indirect 134, add, 1
  word_indirect 135, addc, 1
  word_indirect 136, sub, 1
  word_indirect 137, subc, 1
  word_indirect 138, cmp, 1
  word_indirect 139, xor, 1
  word_indirect 140, and, 1
  word_indirect 141, or, 1
  mov r5, #0xffff
  jmpr cc_uc, .Lreturn
.Lword_done:
  mov r5, psw
  and r5, #31
  mov r2, #0x4002
  cmp r12, #134
  jmpr cc_ult, .Lword_address
  add r2, #2
.Lword_address:
  cmp r1, r2
  jmpr cc_ne, .Lcorrupted
  mov r1, r14
.Ldone:
  and r5, #31
  // Check the source GPR and adjacent words without changing captured flags.
  cmp r14, r3
  jmpr cc_ne, .Lcorrupted
  cmp r1, r14
  jmpr cc_ne, .Lcorrupted
  mov r2, 0x4000
  cmp r2, #0xc35a
  jmpr cc_ne, .Lcorrupted
  mov r2, 0x4004
  cmp r2, #0x5ac3
  jmpr cc_ne, .Lcorrupted
  cmp r12, #114
  jmpr cc_uge, .Lcheck_source
  cmp r12, #91
  jmpr cc_ult, .Lcheck_word_sfr
  cmp r12, #99
  jmpr cc_ult, .Lcheck_source
  jmpr cc_uc, .Lreturn
.Lcheck_word_sfr:
  cmp r12, #48
  jmpr cc_ult, .Lcheck_gpr_form
  cmp r12, #56
  jmpr cc_ult, .Lcheck_source
  jmpr cc_uc, .Lreturn
.Lcheck_gpr_form:
  cmp r12, #23
  jmpr cc_uge, .Lreturn
  cmp r12, #15
  jmpr cc_uge, .Lcheck_source
  cmp r12, #8
  jmpr cc_uge, .Lreturn
.Lcheck_source:
  mov r2, 0x4002
  cmp r2, r14
  jmpr cc_eq, .Lreturn
.Lcorrupted:
  or r5, #32
.Lreturn:
  pop dpp1
  pop mdl
  pop mdh
  pop mdc
  pop psw
  RETURN
.size _test_alu_direct_memory, .-_test_alu_direct_memory

.section .alu.operand,"aw",@progbits
.p2align 1
  .short 0xc35a
.Loperand:
  .short 0
  .short 0x5ac3
