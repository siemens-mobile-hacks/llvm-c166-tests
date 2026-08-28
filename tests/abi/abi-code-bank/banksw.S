// CrossView does not model an application-specific code-bank register.  This
// veneer models the target-independent TASKING Classic __banksw state machine:
// it checks the public input contract, records the logical target bank, calls
// through R4:R5, and restores a non-zero caller bank after the nested return.
// It deliberately does not pretend to be a production hardware bank switch.
        .section .c166.banksw,"ax",@progbits
        .globl  __banksw
        .type   __banksw,@function
__banksw:
        // The caller must save the same current/target word both in R3 and in
        // the hidden first user-stack slot.
        mov     r1, [r0]
        cmp     r3, r1
        jmpr    cc_ne, .Lbad_bank_call

        // A non-zero current bank must match the bank in which the caller is
        // logically executing.  Bank zero is special: non-banked code does
        // not request restoration and may execute with any bank selected.
        mov     r2, r3
        shr     r2, #8
        mov     r1, #0
        cmp     r2, r1
        jmpr    cc_eq, .Lcurrent_bank_ok
        extp    pag(_banksw_active_bank), #1
        mov     r1, pof(_banksw_active_bank)
        cmp     r2, r1
        jmpr    cc_eq, .Lcurrent_bank_ok
        mov     r1, #2
        extp    pag(_banksw_failure), #1
        mov     pof(_banksw_failure), r1

.Lcurrent_bank_ok:
        // Select the target bank in the logical ISS model.
        mov     r1, r3
        mov     r2, #255
        and     r1, r2
        extp    pag(_banksw_active_bank), #1
        mov     pof(_banksw_active_bank), r1

        extp    pag(_banksw_call_count), #1
        mov     r1, pof(_banksw_call_count)
        add     r1, #1
        extp    pag(_banksw_call_count), #1
        mov     pof(_banksw_call_count), r1

        extp    pag(_banksw_depth), #1
        mov     r2, pof(_banksw_depth)
        add     r2, #1
        extp    pag(_banksw_depth), #1
        mov     pof(_banksw_depth), r2
        extp    pag(_banksw_max_depth), #1
        mov     r1, pof(_banksw_max_depth)
        cmp     r2, r1
        jmpr    cc_ule, .Ldepth_recorded
        extp    pag(_banksw_max_depth), #1
        mov     pof(_banksw_max_depth), r2

.Ldepth_recorded:
        // __icall tail-dispatches through R4:R5.  Because it is reached by a
        // real CALLS here, the target's RETS returns to .Lbanked_return first;
        // this veneer can then restore the caller before its own RETS.
        push    r3
        calls   seg(__icall), sof(__icall)

.Lbanked_return:
        pop     r3

        extp    pag(_banksw_depth), #1
        mov     r1, pof(_banksw_depth)
        sub     r1, #1
        extp    pag(_banksw_depth), #1
        mov     pof(_banksw_depth), r1

        mov     r2, r3
        shr     r2, #8
        mov     r1, #0
        cmp     r2, r1
        jmpr    cc_eq, .Lreturn_to_caller
        extp    pag(_banksw_active_bank), #1
        mov     pof(_banksw_active_bank), r2
        extp    pag(_banksw_restore_count), #1
        mov     r1, pof(_banksw_restore_count)
        add     r1, #1
        extp    pag(_banksw_restore_count), #1
        mov     pof(_banksw_restore_count), r1

.Lreturn_to_caller:
        rets

.Lbad_bank_call:
        mov     r1, #1
        extp    pag(_banksw_failure), #1
        mov     pof(_banksw_failure), r1
        mov     r4, #57005
        rets
        .size   __banksw, .-__banksw

// Seed and verify the state that the manual requires __banksw to preserve.
// The C driver below this wrapper performs a three-level 0->1->2->1 chain.
        .text
        .globl  _code_bank_state_probe
        .type   _code_bank_state_probe,@function
_code_bank_state_probe:
        mov     r1, sp
        push    r1
        push    r0
        push    r6
        push    r7
        push    r8
        push    r9
        mov     r1, dpp0
        push    r1
        mov     r1, dpp1
        push    r1
        mov     r1, dpp2
        push    r1
        mov     r1, dpp3
        push    r1

        mov     r6, #26134
        mov     r7, #30487
        mov     r8, #34840
        mov     r9, #39193
        calls   seg(_code_bank_nested_driver), sof(_code_bank_nested_driver)
        mov     r10, r4
        mov     r11, #0

        mov     r1, #26134
        cmp     r6, r1
        jmpr    cc_eq, .Lr6_ok
        mov     r11, #1
.Lr6_ok:
        mov     r1, #30487
        cmp     r7, r1
        jmpr    cc_eq, .Lr7_ok
        mov     r11, #1
.Lr7_ok:
        mov     r1, #34840
        cmp     r8, r1
        jmpr    cc_eq, .Lr8_ok
        mov     r11, #1
.Lr8_ok:
        mov     r1, #39193
        cmp     r9, r1
        jmpr    cc_eq, .Lr9_ok
        mov     r11, #1
.Lr9_ok:

        pop     r1
        mov     r2, dpp3
        cmp     r1, r2
        jmpr    cc_eq, .Ldpp3_ok
        mov     r11, #1
.Ldpp3_ok:
        pop     r1
        mov     r2, dpp2
        cmp     r1, r2
        jmpr    cc_eq, .Ldpp2_ok
        mov     r11, #1
.Ldpp2_ok:
        pop     r1
        mov     r2, dpp1
        cmp     r1, r2
        jmpr    cc_eq, .Ldpp1_ok
        mov     r11, #1
.Ldpp1_ok:
        pop     r1
        mov     r2, dpp0
        cmp     r1, r2
        jmpr    cc_eq, .Ldpp0_ok
        mov     r11, #1
.Ldpp0_ok:

        pop     r9
        pop     r8
        pop     r7
        pop     r6
        pop     r1
        cmp     r0, r1
        jmpr    cc_eq, .Lr0_ok
        mov     r11, #1
.Lr0_ok:
        pop     r1
        mov     r2, sp
        cmp     r1, r2
        jmpr    cc_eq, .Lsp_ok
        mov     r11, #1
.Lsp_ok:
        mov     r1, #0
        cmp     r11, r1
        jmpr    cc_eq, .Lstate_ok
        mov     r4, #49374
        rets
.Lstate_ok:
        mov     r4, r10
        rets
        .size   _code_bank_state_probe, .-_code_bank_state_probe
