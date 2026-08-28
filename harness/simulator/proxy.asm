$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK
; simulator.sh supplies MODEL(...) directly to a166.

        NAME    LLVM_PROXY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR  SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC  _llvm_entry_proxy
        PUBLIC  _llvm_medium_entry_proxy
        PUBLIC  _llvm_medium_huge_proxy
        PUBLIC  _llvm_words_proxy0
        PUBLIC  _llvm_words_proxy1
        PUBLIC  _llvm_words_proxy2
        PUBLIC  _llvm_words_proxy3
        PUBLIC  _llvm_words_proxy4
        PUBLIC  _llvm_words_proxy5
        PUBLIC  _llvm_words_proxy6
        PUBLIC  _llvm_words_proxy7
        PUBLIC  _llvm_words_proxy8
        PUBLIC  _llvm_words_proxy9
        PUBLIC  _llvm_words_proxy10
        PUBLIC  _llvm_reverse_words_proxy
        PUBLIC  __icall
        PUBLIC  _llvm_proxy_s8_0
        PUBLIC  _llvm_proxy_s8_1
        PUBLIC  _llvm_proxy_s8_2
        PUBLIC  _llvm_proxy_s8_3
        PUBLIC  _llvm_proxy_s8_4
        PUBLIC  _llvm_proxy_u8_0
        PUBLIC  _llvm_proxy_u8_1
        PUBLIC  _llvm_proxy_u8_2
        PUBLIC  _llvm_proxy_u8_3
        PUBLIC  _llvm_proxy_u8_4
        PUBLIC  _llvm_proxy_u32_0
        PUBLIC  _llvm_proxy_u32_1
        PUBLIC  _llvm_proxy_u32_2
        PUBLIC  _llvm_proxy_u32_3
        PUBLIC  _llvm_proxy_ptr_0
        PUBLIC  _llvm_proxy_ptr_1
        PUBLIC  _llvm_proxy_ptr_2
        PUBLIC  _llvm_proxy_ptr_3
        PUBLIC  _llvm_proxy_fn_0
        PUBLIC  _llvm_proxy_fn_1
        PUBLIC  _llvm_proxy_fn_2
        PUBLIC  _llvm_proxy_fn_3
        PUBLIC  _llvm_reverse_mixed_proxy
        PUBLIC  _llvm_proxy_enum5
        PUBLIC  _llvm_reverse_enum5_proxy
        PUBLIC  _llvm_recursive_state_proxy
        PUBLIC  _llvm_recursive_tasking_state_proxy
        PUBLIC  _llvm_call_tasking_state_proxy
        PUBLIC  _llvm_recursion_leaf_bridge
        PUBLIC  _llvm_return1_proxy
        PUBLIC  _llvm_return2_proxy
        PUBLIC  _llvm_return3_proxy
        PUBLIC  _llvm_return4_proxy
        PUBLIC  _llvm_return5_proxy
        PUBLIC  _llvm_return6_proxy
        PUBLIC  _llvm_return7_proxy
        PUBLIC  _llvm_return8_proxy
        PUBLIC  _llvm_return_tail_proxy
        PUBLIC  _tasking_varargs_aggregate_abi_proxy
        PUBLIC  _llvm_varargs_aggregate_abi_proxy
        PUBLIC  _llvm_varargs_aggregate_reverse_proxy
        PUBLIC  _llvm_f32_to_f64_proxy
        PUBLIC  _llvm_f64_to_f32_proxy

; TASKING-generated indirect huge calls name __icall.  The public helper is
; huge in Large but near in Medium.  This must be a frame-free tail jump:
; compiler-rt consumes the caller's original return frame while synthesizing
; the target address, so an intermediate CALLA/CALLS frame is not transparent.
@IF( @TASKING_MODEL_IS_MEDIUM )
__icall PROC NEAR
        JMPA    cc_UC,0EF00h
__icall ENDP
@ELSE
__icall PROC FAR
        JMPS    17h,0FF00h
__icall ENDP
@ENDI

_llvm_entry_proxy PROC FAR
        CALLS   10h,0000h
        RETS
_llvm_entry_proxy ENDP

; The same C host sources call this symbol in Medium after the runner's
; model adapter renames llvm_entry_proxy.  A near CALLA frame is preserved
; across the fixed LLVM entry trampoline at 0xC000.
_llvm_medium_entry_proxy PROC NEAR
        CALLA   cc_UC,0C000h
        RET
_llvm_medium_entry_proxy ENDP

; Stable explicitly-huge route used by Medium ABI tests.  The target lives
; outside the first code segment and therefore cannot be reached as a near
; function even though the surrounding program uses the Medium model.
_llvm_medium_huge_proxy PROC FAR
        CALLS   18h,0100h
        RETS
_llvm_medium_huge_proxy ENDP

; Common 0--10 word ordinary-call bank.  Large targets live in the fixed
; 0x10 LLVM segment; Medium reuses the same source functions in the fixed
; first-segment overlay.  The stubs preserve the complete register/user-stack
; boundary and differ only in the model-required call/return class.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_words_proxy0 PROC NEAR
        CALLA   cc_UC,0C100h
        RET
_llvm_words_proxy0 ENDP
_llvm_words_proxy1 PROC NEAR
        CALLA   cc_UC,0C120h
        RET
_llvm_words_proxy1 ENDP
_llvm_words_proxy2 PROC NEAR
        CALLA   cc_UC,0C1A0h
        RET
_llvm_words_proxy2 ENDP
_llvm_words_proxy3 PROC NEAR
        CALLA   cc_UC,0C280h
        RET
_llvm_words_proxy3 ENDP
_llvm_words_proxy4 PROC NEAR
        CALLA   cc_UC,0C3C0h
        RET
_llvm_words_proxy4 ENDP
_llvm_words_proxy5 PROC NEAR
        CALLA   cc_UC,0C560h
        RET
_llvm_words_proxy5 ENDP
_llvm_words_proxy6 PROC NEAR
        CALLA   cc_UC,0C760h
        RET
_llvm_words_proxy6 ENDP
_llvm_words_proxy7 PROC NEAR
        CALLA   cc_UC,0C9C0h
        RET
_llvm_words_proxy7 ENDP
_llvm_words_proxy8 PROC NEAR
        CALLA   cc_UC,0CC80h
        RET
_llvm_words_proxy8 ENDP
_llvm_words_proxy9 PROC NEAR
        CALLA   cc_UC,0CFE0h
        RET
_llvm_words_proxy9 ENDP
_llvm_words_proxy10 PROC NEAR
        CALLA   cc_UC,0D3C0h
        RET
_llvm_words_proxy10 ENDP
_llvm_reverse_words_proxy PROC NEAR
        CALLA   cc_UC,0D800h
        RET
_llvm_reverse_words_proxy ENDP
@ELSE
_llvm_words_proxy0 PROC FAR
        CALLS   12h,0000h
        RETS
_llvm_words_proxy0 ENDP
_llvm_words_proxy1 PROC FAR
        CALLS   12h,1000h
        RETS
_llvm_words_proxy1 ENDP
_llvm_words_proxy2 PROC FAR
        CALLS   12h,2000h
        RETS
_llvm_words_proxy2 ENDP
_llvm_words_proxy3 PROC FAR
        CALLS   12h,3000h
        RETS
_llvm_words_proxy3 ENDP
_llvm_words_proxy4 PROC FAR
        CALLS   12h,4000h
        RETS
_llvm_words_proxy4 ENDP
_llvm_words_proxy5 PROC FAR
        CALLS   12h,5000h
        RETS
_llvm_words_proxy5 ENDP
_llvm_words_proxy6 PROC FAR
        CALLS   12h,6000h
        RETS
_llvm_words_proxy6 ENDP
_llvm_words_proxy7 PROC FAR
        CALLS   12h,7000h
        RETS
_llvm_words_proxy7 ENDP
_llvm_words_proxy8 PROC FAR
        CALLS   12h,8000h
        RETS
_llvm_words_proxy8 ENDP
_llvm_words_proxy9 PROC FAR
        CALLS   12h,9000h
        RETS
_llvm_words_proxy9 ENDP
_llvm_words_proxy10 PROC FAR
        CALLS   12h,0A000h
        RETS
_llvm_words_proxy10 ENDP
_llvm_reverse_words_proxy PROC FAR
        CALLS   12h,0B000h
        RETS
_llvm_reverse_words_proxy ENDP
@ENDI

; Reusable generated mixed-signature ABI call bank.  These transparent stubs
; preserve the Classic argument registers/stack exactly and let both the
; deterministic slot matrix and runner-seeded call stress share one proxy.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_proxy_s8_0 PROC NEAR
        CALLA cc_UC,0C100h
        RET
_llvm_proxy_s8_0 ENDP
_llvm_proxy_s8_1 PROC NEAR
        CALLA cc_UC,0C200h
        RET
_llvm_proxy_s8_1 ENDP
_llvm_proxy_s8_2 PROC NEAR
        CALLA cc_UC,0C300h
        RET
_llvm_proxy_s8_2 ENDP
_llvm_proxy_s8_3 PROC NEAR
        CALLA cc_UC,0C400h
        RET
_llvm_proxy_s8_3 ENDP
_llvm_proxy_s8_4 PROC NEAR
        CALLA cc_UC,0C500h
        RET
_llvm_proxy_s8_4 ENDP
_llvm_proxy_u8_0 PROC NEAR
        CALLA cc_UC,0C600h
        RET
_llvm_proxy_u8_0 ENDP
_llvm_proxy_u8_1 PROC NEAR
        CALLA cc_UC,0C700h
        RET
_llvm_proxy_u8_1 ENDP
_llvm_proxy_u8_2 PROC NEAR
        CALLA cc_UC,0C800h
        RET
_llvm_proxy_u8_2 ENDP
_llvm_proxy_u8_3 PROC NEAR
        CALLA cc_UC,0C900h
        RET
_llvm_proxy_u8_3 ENDP
_llvm_proxy_u8_4 PROC NEAR
        CALLA cc_UC,0CA00h
        RET
_llvm_proxy_u8_4 ENDP
_llvm_proxy_u32_0 PROC NEAR
        CALLA cc_UC,0CB00h
        RET
_llvm_proxy_u32_0 ENDP
_llvm_proxy_u32_1 PROC NEAR
        CALLA cc_UC,0CC00h
        RET
_llvm_proxy_u32_1 ENDP
_llvm_proxy_u32_2 PROC NEAR
        CALLA cc_UC,0CD00h
        RET
_llvm_proxy_u32_2 ENDP
_llvm_proxy_u32_3 PROC NEAR
        CALLA cc_UC,0CE00h
        RET
_llvm_proxy_u32_3 ENDP
_llvm_proxy_ptr_0 PROC NEAR
        CALLA cc_UC,0CF00h
        RET
_llvm_proxy_ptr_0 ENDP
_llvm_proxy_ptr_1 PROC NEAR
        CALLA cc_UC,0D000h
        RET
_llvm_proxy_ptr_1 ENDP
_llvm_proxy_ptr_2 PROC NEAR
        CALLA cc_UC,0D100h
        RET
_llvm_proxy_ptr_2 ENDP
_llvm_proxy_ptr_3 PROC NEAR
        CALLA cc_UC,0D200h
        RET
_llvm_proxy_ptr_3 ENDP
_llvm_proxy_fn_0 PROC NEAR
        CALLA cc_UC,0D300h
        RET
_llvm_proxy_fn_0 ENDP
_llvm_proxy_fn_1 PROC NEAR
        CALLA cc_UC,0D400h
        RET
_llvm_proxy_fn_1 ENDP
_llvm_proxy_fn_2 PROC NEAR
        CALLA cc_UC,0D500h
        RET
_llvm_proxy_fn_2 ENDP
_llvm_proxy_fn_3 PROC NEAR
        CALLA cc_UC,0D600h
        RET
_llvm_proxy_fn_3 ENDP
_llvm_reverse_mixed_proxy PROC NEAR
        CALLA cc_UC,0D700h
        RET
_llvm_reverse_mixed_proxy ENDP
_llvm_proxy_enum5 PROC NEAR
        CALLA cc_UC,0DCB6h
        RET
_llvm_proxy_enum5 ENDP
_llvm_reverse_enum5_proxy PROC NEAR
        CALLA cc_UC,0DDD4h
        RET
_llvm_reverse_enum5_proxy ENDP
@ELSE
_llvm_proxy_s8_0 PROC FAR
        CALLS 10h,00100h
        RETS
_llvm_proxy_s8_0 ENDP
_llvm_proxy_s8_1 PROC FAR
        CALLS 10h,00200h
        RETS
_llvm_proxy_s8_1 ENDP
_llvm_proxy_s8_2 PROC FAR
        CALLS 10h,00300h
        RETS
_llvm_proxy_s8_2 ENDP
_llvm_proxy_s8_3 PROC FAR
        CALLS 10h,00400h
        RETS
_llvm_proxy_s8_3 ENDP
_llvm_proxy_s8_4 PROC FAR
        CALLS 10h,00500h
        RETS
_llvm_proxy_s8_4 ENDP
_llvm_proxy_u8_0 PROC FAR
        CALLS 10h,00600h
        RETS
_llvm_proxy_u8_0 ENDP
_llvm_proxy_u8_1 PROC FAR
        CALLS 10h,00700h
        RETS
_llvm_proxy_u8_1 ENDP
_llvm_proxy_u8_2 PROC FAR
        CALLS 10h,00800h
        RETS
_llvm_proxy_u8_2 ENDP
_llvm_proxy_u8_3 PROC FAR
        CALLS 10h,00900h
        RETS
_llvm_proxy_u8_3 ENDP
_llvm_proxy_u8_4 PROC FAR
        CALLS 10h,00A00h
        RETS
_llvm_proxy_u8_4 ENDP
_llvm_proxy_u32_0 PROC FAR
        CALLS 10h,00B00h
        RETS
_llvm_proxy_u32_0 ENDP
_llvm_proxy_u32_1 PROC FAR
        CALLS 10h,00C00h
        RETS
_llvm_proxy_u32_1 ENDP
_llvm_proxy_u32_2 PROC FAR
        CALLS 10h,00D00h
        RETS
_llvm_proxy_u32_2 ENDP
_llvm_proxy_u32_3 PROC FAR
        CALLS 10h,00E00h
        RETS
_llvm_proxy_u32_3 ENDP
_llvm_proxy_ptr_0 PROC FAR
        CALLS 10h,00F00h
        RETS
_llvm_proxy_ptr_0 ENDP
_llvm_proxy_ptr_1 PROC FAR
        CALLS 10h,01000h
        RETS
_llvm_proxy_ptr_1 ENDP
_llvm_proxy_ptr_2 PROC FAR
        CALLS 10h,01100h
        RETS
_llvm_proxy_ptr_2 ENDP
_llvm_proxy_ptr_3 PROC FAR
        CALLS 10h,01200h
        RETS
_llvm_proxy_ptr_3 ENDP
_llvm_proxy_fn_0 PROC FAR
        CALLS 10h,01300h
        RETS
_llvm_proxy_fn_0 ENDP
_llvm_proxy_fn_1 PROC FAR
        CALLS 10h,01400h
        RETS
_llvm_proxy_fn_1 ENDP
_llvm_proxy_fn_2 PROC FAR
        CALLS 10h,01500h
        RETS
_llvm_proxy_fn_2 ENDP
_llvm_proxy_fn_3 PROC FAR
        CALLS 10h,01600h
        RETS
_llvm_proxy_fn_3 ENDP
_llvm_reverse_mixed_proxy PROC FAR
        CALLS 10h,01700h
        RETS
_llvm_reverse_mixed_proxy ENDP
_llvm_proxy_enum5 PROC FAR
        CALLS 10h,02000h
        RETS
_llvm_proxy_enum5 ENDP
_llvm_reverse_enum5_proxy PROC FAR
        CALLS 10h,03000h
        RETS
_llvm_reverse_enum5_proxy ENDP
@ENDI

; Common f32/f64 width-conversion ABI probes.  The assembly caller exposes the
; public MSW-first argument/result words while the production helpers execute
; inside the LLVM overlay.  User-stack allocation is independent of the
; system-stack call-frame width; only CALL/RET class changes by model.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f32_to_f64_proxy PROC NEAR
@ELSE
_llvm_f32_to_f64_proxy PROC FAR
@ENDI
        SUB     R0,#07h
        SUB     R0,#05h
        MOV     [R0],R12
        MOV     [R0+#02h],R13
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C100h
@ELSE
        CALLS   10h,00100h
@ENDI
        MOV     R10,[R4]
        MOV     R11,[R4+#02h]
        MOV     R1,[R4+#04h]
        MOV     R2,[R4+#06h]
        EXTP    #PAG _llvm_float_width_observed,#02h
        MOV     _llvm_float_width_observed,R10
        MOV     _llvm_float_width_observed+02h,R11
        EXTP    #PAG _llvm_float_width_observed,#02h
        MOV     _llvm_float_width_observed+04h,R1
        MOV     _llvm_float_width_observed+06h,R2
        ADD     R0,#07h
        ADD     R0,#05h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f32_to_f64_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_to_f32_proxy PROC NEAR
@ELSE
_llvm_f64_to_f32_proxy PROC FAR
@ENDI
        SUB     R0,#07h
        SUB     R0,#01h
        MOV     [R0],R12
        MOV     [R0+#02h],R13
        MOV     [R0+#04h],R14
        MOV     [R0+#06h],R15
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C900h
@ELSE
        CALLS   10h,00900h
@ENDI
        EXTP    #PAG _llvm_float_width_observed,#02h
        MOV     _llvm_float_width_observed,R4
        MOV     _llvm_float_width_observed+02h,R5
        ADD     R0,#07h
        ADD     R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_to_f32_proxy ENDP

; Common aggregate-varargs ABI callers.  The stream is the PDF-defined
; pair2[4], chars3[4], packed3[3]+pad[1], tail[2] sequence.  TASKING Build
; 753's C caller omits the packed padding, so the assembly path deliberately
; constructs the authoritative stream for both compilers and both models.
@IF( @TASKING_MODEL_IS_MEDIUM )
_tasking_varargs_aggregate_abi_proxy PROC NEAR
@ELSE
_tasking_varargs_aggregate_abi_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_SMALL )
        MOV     [-R0],R6
        MOV     [-R0],R7
        SUB     R0,#0EH

        MOV     R6,[R12]
        MOV     R7,[R12+#02H]
        MOV     [R0],R6
        MOV     [R0+#02H],R7

        MOVB    RL6,[R13]
        MOVB    RH6,[R13+#01H]
        MOVB    RL7,[R13+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        MOV     R6,#00H
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOV     [R0+#0CH],R15
@ELSE
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R5,[R0+#04H]
        MOV     [-R0],R6
        MOV     [-R0],R7
        SUB     R0,#0EH

        EXTP    R13,#02H
        MOV     R6,[R12]
        MOV     R7,[R12+#02H]
        MOV     [R0],R6
        MOV     [R0+#02H],R7

        EXTP    R15,#03H
        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        EXTP    R2,#03H
        MOVB    RL6,[R1]
        MOVB    RH6,[R1+#01H]
        MOVB    RL7,[R1+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        MOV     R6,#00H
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOV     [R0+#0CH],R5
@ENDI
        MOV     R12,#01357H
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,08100h
@ELSE
        CALLS   09h,08000h
@ENDI
        ADD     R0,#0EH
        MOV     R7,[R0+]
        MOV     R6,[R0+]
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_tasking_varargs_aggregate_abi_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_aggregate_abi_proxy PROC NEAR
@ELSE
_llvm_varargs_aggregate_abi_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_SMALL )
        MOV     [-R0],R6
        MOV     [-R0],R7
        SUB     R0,#0EH

        MOV     R6,[R12]
        MOV     R7,[R12+#02H]
        MOV     [R0],R6
        MOV     [R0+#02H],R7

        MOVB    RL6,[R13]
        MOVB    RH6,[R13+#01H]
        MOVB    RL7,[R13+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        MOV     R6,#00H
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOV     [R0+#0CH],R15
@ELSE
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R5,[R0+#04H]
        MOV     [-R0],R6
        MOV     [-R0],R7
        SUB     R0,#0EH

        EXTP    R13,#02H
        MOV     R6,[R12]
        MOV     R7,[R12+#02H]
        MOV     [R0],R6
        MOV     [R0+#02H],R7

        EXTP    R15,#03H
        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        EXTP    R2,#03H
        MOVB    RL6,[R1]
        MOVB    RH6,[R1+#01H]
        MOVB    RL7,[R1+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        MOV     R6,#00H
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOV     [R0+#0CH],R5
@ENDI
        MOV     R12,#01357H
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C100h
@ELSE
        CALLS   10h,0100h
@ENDI
        ADD     R0,#0EH
        MOV     R7,[R0+]
        MOV     R6,[R0+]
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_aggregate_abi_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_aggregate_reverse_proxy PROC NEAR
        CALLA   cc_UC,0C500h
        RET
_llvm_varargs_aggregate_reverse_proxy ENDP
@ELSE
_llvm_varargs_aggregate_reverse_proxy PROC FAR
        CALLS   10h,0500h
        RETS
_llvm_varargs_aggregate_reverse_proxy ENDP
@ENDI

; Common aggregate-result call bank.  Caller-reserved result storage and the
; complete user-stack tail pass through unchanged; only call-frame width and
; target placement vary between Large and Medium.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_return1_proxy PROC NEAR
        CALLA   cc_UC,0C100h
        RET
_llvm_return1_proxy ENDP
_llvm_return2_proxy PROC NEAR
        CALLA   cc_UC,0C200h
        RET
_llvm_return2_proxy ENDP
_llvm_return3_proxy PROC NEAR
        CALLA   cc_UC,0C300h
        RET
_llvm_return3_proxy ENDP
_llvm_return4_proxy PROC NEAR
        CALLA   cc_UC,0C400h
        RET
_llvm_return4_proxy ENDP
_llvm_return5_proxy PROC NEAR
        CALLA   cc_UC,0C500h
        RET
_llvm_return5_proxy ENDP
_llvm_return6_proxy PROC NEAR
        CALLA   cc_UC,0C600h
        RET
_llvm_return6_proxy ENDP
_llvm_return7_proxy PROC NEAR
        CALLA   cc_UC,0C700h
        RET
_llvm_return7_proxy ENDP
_llvm_return8_proxy PROC NEAR
        CALLA   cc_UC,0C800h
        RET
_llvm_return8_proxy ENDP
_llvm_return_tail_proxy PROC NEAR
        CALLA   cc_UC,0C900h
        RET
_llvm_return_tail_proxy ENDP
@ELSE
_llvm_return1_proxy PROC FAR
        CALLS   10h,0100h
        RETS
_llvm_return1_proxy ENDP
_llvm_return2_proxy PROC FAR
        CALLS   10h,0200h
        RETS
_llvm_return2_proxy ENDP
_llvm_return3_proxy PROC FAR
        CALLS   10h,0300h
        RETS
_llvm_return3_proxy ENDP
_llvm_return4_proxy PROC FAR
        CALLS   10h,0400h
        RETS
_llvm_return4_proxy ENDP
_llvm_return5_proxy PROC FAR
        CALLS   10h,0500h
        RETS
_llvm_return5_proxy ENDP
_llvm_return6_proxy PROC FAR
        CALLS   10h,0600h
        RETS
_llvm_return6_proxy ENDP
_llvm_return7_proxy PROC FAR
        CALLS   10h,0700h
        RETS
_llvm_return7_proxy ENDP
_llvm_return8_proxy PROC FAR
        CALLS   10h,0800h
        RETS
_llvm_return8_proxy ENDP
_llvm_return_tail_proxy PROC FAR
        CALLS   10h,0900h
        RETS
_llvm_return_tail_proxy ENDP
@ENDI

; Common recursion/state call boundary.  It verifies every callee-saved GPR,
; DPP register and both user/system stack pointers around the same recursive
; C kernels.  Only the code-pointer class changes with the memory model.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_recursion_leaf_bridge PROC NEAR
        CALLA   cc_UC,0C100h
        RET
_llvm_recursion_leaf_bridge ENDP

_llvm_recursive_state_proxy PROC NEAR
        MOV     R2,#00h
        CALLA   cc_UC,RECURSION_STATE_CALL
        RET
_llvm_recursive_state_proxy ENDP

_llvm_recursive_tasking_state_proxy PROC NEAR
        MOV     R2,#01h
        CALLA   cc_UC,RECURSION_STATE_CALL
        RET
_llvm_recursive_tasking_state_proxy ENDP

_llvm_call_tasking_state_proxy PROC NEAR
        MOV     R2,#02h
        CALLA   cc_UC,RECURSION_STATE_CALL
        RET
_llvm_call_tasking_state_proxy ENDP

RECURSION_STATE_CALL PROC NEAR
@ELSE
_llvm_recursion_leaf_bridge PROC FAR
        CALLS   10h,0100h
        RETS
_llvm_recursion_leaf_bridge ENDP

_llvm_recursive_state_proxy PROC FAR
        MOV     R2,#00h
        CALLS   SEG RECURSION_STATE_CALL,RECURSION_STATE_CALL
        RETS
_llvm_recursive_state_proxy ENDP

_llvm_recursive_tasking_state_proxy PROC FAR
        MOV     R2,#01h
        CALLS   SEG RECURSION_STATE_CALL,RECURSION_STATE_CALL
        RETS
_llvm_recursive_tasking_state_proxy ENDP

_llvm_call_tasking_state_proxy PROC FAR
        MOV     R2,#02h
        CALLS   SEG RECURSION_STATE_CALL,RECURSION_STATE_CALL
        RETS
_llvm_call_tasking_state_proxy ENDP

RECURSION_STATE_CALL PROC FAR
@ENDI
        MOV     R1,SP
        PUSH    R1
        PUSH    R0
        PUSH    R6
        PUSH    R7
        PUSH    R8
        PUSH    R9
        MOV     R1,DPP0
        PUSH    R1
        MOV     R1,DPP1
        PUSH    R1
        MOV     R1,DPP2
        PUSH    R1
        MOV     R1,DPP3
        PUSH    R1

        CMP     R2,#00h
        JMPR    cc_EQ,RECURSION_STATE_CALL_LLVM
        CMP     R2,#01h
        JMPR    cc_EQ,RECURSION_STATE_CALL_LLVM_TASKING
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0D700h
@ELSE
        CALLS   10h,6000h
@ENDI
        JMPR    cc_UC,RECURSION_STATE_CALL_DONE
RECURSION_STATE_CALL_LLVM:
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C300h
@ELSE
        CALLS   10h,2000h
@ENDI
        JMPR    cc_UC,RECURSION_STATE_CALL_DONE
RECURSION_STATE_CALL_LLVM_TASKING:
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0CD00h
@ELSE
        CALLS   10h,4000h
@ENDI
RECURSION_STATE_CALL_DONE:
        PUSH    R4
        PUSH    R5
        MOV     R3,#00h
        POP     R11
        POP     R10

        POP     R1
        MOV     R2,DPP3
        CMP     R1,R2
        JMPR    cc_EQ,RECURSION_STATE_DPP3_OK
        MOV     R3,#01h
RECURSION_STATE_DPP3_OK:
        POP     R1
        MOV     R2,DPP2
        CMP     R1,R2
        JMPR    cc_EQ,RECURSION_STATE_DPP2_OK
        MOV     R3,#01h
RECURSION_STATE_DPP2_OK:
        POP     R1
        MOV     R2,DPP1
        CMP     R1,R2
        JMPR    cc_EQ,RECURSION_STATE_DPP1_OK
        MOV     R3,#01h
RECURSION_STATE_DPP1_OK:
        POP     R1
        MOV     R2,DPP0
        CMP     R1,R2
        JMPR    cc_EQ,RECURSION_STATE_DPP0_OK
        MOV     R3,#01h
RECURSION_STATE_DPP0_OK:

        POP     R1
        CMP     R1,R9
        JMPR    cc_EQ,RECURSION_STATE_R9_OK
        MOV     R3,#01h
RECURSION_STATE_R9_OK:
        MOV     R9,R1
        POP     R1
        CMP     R1,R8
        JMPR    cc_EQ,RECURSION_STATE_R8_OK
        MOV     R3,#01h
RECURSION_STATE_R8_OK:
        MOV     R8,R1
        POP     R1
        CMP     R1,R7
        JMPR    cc_EQ,RECURSION_STATE_R7_OK
        MOV     R3,#01h
RECURSION_STATE_R7_OK:
        MOV     R7,R1
        POP     R1
        CMP     R1,R6
        JMPR    cc_EQ,RECURSION_STATE_R6_OK
        MOV     R3,#01h
RECURSION_STATE_R6_OK:
        MOV     R6,R1

        POP     R1
        CMP     R1,R0
        JMPR    cc_EQ,RECURSION_STATE_R0_OK
        MOV     R3,#01h
RECURSION_STATE_R0_OK:
        MOV     R0,R1
        POP     R1
        MOV     R2,SP
        CMP     R1,R2
        JMPR    cc_EQ,RECURSION_STATE_SP_OK
        MOV     R3,#01h
RECURSION_STATE_SP_OK:

        CMP     R3,#00h
        JMPR    cc_EQ,RECURSION_STATE_ALL_OK
        MOV     R4,#0C0DEh
        MOV     R5,#0DEADh
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
RECURSION_STATE_ALL_OK:
        MOV     R4,R10
        MOV     R5,R11
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
RECURSION_STATE_CALL ENDP

LLVM_PROXY_PR  ENDS

; Shared same-segment code arena for Classic _near function ABI tests.  The
; TASKING image owns the reservation and exports the near-text anchor; the
; universal runner overlays LLVM ELF sections at the same addresses.
LLVM_NEAR_CALLERS_RESERVATION SECTION CODE WORD PUBLIC 'LLVMNEARCALLERS'
        DS      04000h
LLVM_NEAR_CALLERS_RESERVATION ENDS

LLVM_NEAR_TEXT_RESERVATION SECTION CODE WORD PUBLIC 'LLVMNEARTEXT'
        PUBLIC  _llvm_near_code_base
_llvm_near_code_base PROC NEAR
        RET
        DS      03FFEh
_llvm_near_code_base ENDP
LLVM_NEAR_TEXT_RESERVATION ENDS

; One model-wide first-segment reservation, shared by every Medium case.
; Intel HEX overlay replaces it with the LLVM near entry/text image; cases
; never allocate or resize this region themselves.
LLVM_MEDIUM_TEXT_RESERVATION SECTION CODE WORD PUBLIC 'LLVMMEDIUMTEXT'
        DS      00900h
        ; This real TASKING near symbol gives mixed-compiler tests a COF16
        ; function address without relying on an integer-to-function-pointer
        ; cast.  The LLVM overlay replaces its placeholder RET at 0xC900.
        PUBLIC  _llvm_medium_near_proxy
_llvm_medium_near_proxy PROC NEAR
        RET
        DS      026FEh
_llvm_medium_near_proxy ENDP
LLVM_MEDIUM_TEXT_RESERVATION ENDS

; Shared observation slot for f32/f64 public-word ABI probes.  It occupies the
; common harness state belongs to the fixed CFAR arena, not to a fixture data
; class.  Keeping it out of FLOATVALUES makes user-stack placement independent
; of whether a particular test happens to define floating-point data.
FLOAT_WIDTH_VALUES SECTION DATA WORD PUBLIC 'CFAR'
_llvm_float_width_observed LABEL WORD
        DS      008h
        PUBLIC  _llvm_float_width_observed
FLOAT_WIDTH_VALUES ENDS

; TASKING object sections have a 16-bit size field.  Each fixed 64 KiB ISS
; segment is therefore covered by a sparse 0xfffe-byte body and a two-byte
; tail.  The full 2 MiB ROM and RAM maps are contiguous, while Intel HEX only
; contains the 128 explicit tail bytes instead of a 4 MiB fill.
LLVM_ROM_BODY_00 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_00 ENDS

LLVM_ROM_TAIL_00 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_00 ENDS

LLVM_ROM_BODY_01 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_01 ENDS

LLVM_ROM_TAIL_01 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_01 ENDS

LLVM_ROM_BODY_02 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_02 ENDS

LLVM_ROM_TAIL_02 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_02 ENDS

LLVM_ROM_BODY_03 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_03 ENDS

LLVM_ROM_TAIL_03 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_03 ENDS

LLVM_ROM_BODY_04 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_04 ENDS

LLVM_ROM_TAIL_04 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_04 ENDS

LLVM_ROM_BODY_05 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_05 ENDS

LLVM_ROM_TAIL_05 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_05 ENDS

LLVM_ROM_BODY_06 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_06 ENDS

LLVM_ROM_TAIL_06 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_06 ENDS

LLVM_ROM_BODY_07 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_07 ENDS

LLVM_ROM_TAIL_07 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_07 ENDS

LLVM_ROM_BODY_08 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_08 ENDS

LLVM_ROM_TAIL_08 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_08 ENDS

LLVM_ROM_BODY_09 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_09 ENDS

LLVM_ROM_TAIL_09 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_09 ENDS

LLVM_ROM_BODY_0A SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0A ENDS

LLVM_ROM_TAIL_0A SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0A ENDS

LLVM_ROM_BODY_0B SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0B ENDS

LLVM_ROM_TAIL_0B SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0B ENDS

LLVM_ROM_BODY_0C SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0C ENDS

LLVM_ROM_TAIL_0C SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0C ENDS

LLVM_ROM_BODY_0D SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0D ENDS

LLVM_ROM_TAIL_0D SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0D ENDS

LLVM_ROM_BODY_0E SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0E ENDS

LLVM_ROM_TAIL_0E SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0E ENDS

LLVM_ROM_BODY_0F SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_0F ENDS

LLVM_ROM_TAIL_0F SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_0F ENDS

LLVM_ROM_BODY_10 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_10 ENDS

LLVM_ROM_TAIL_10 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_10 ENDS

LLVM_ROM_BODY_11 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_11 ENDS

LLVM_ROM_TAIL_11 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_11 ENDS

LLVM_ROM_BODY_12 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_12 ENDS

LLVM_ROM_TAIL_12 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_12 ENDS

LLVM_ROM_BODY_13 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_13 ENDS

LLVM_ROM_TAIL_13 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_13 ENDS

LLVM_ROM_BODY_14 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_14 ENDS

LLVM_ROM_TAIL_14 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_14 ENDS

LLVM_ROM_BODY_15 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_15 ENDS

LLVM_ROM_TAIL_15 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_15 ENDS

LLVM_ROM_BODY_16 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_16 ENDS

LLVM_ROM_TAIL_16 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_16 ENDS

LLVM_ROM_BODY_17 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_17 ENDS

LLVM_ROM_TAIL_17 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_17 ENDS

LLVM_ROM_BODY_18 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_18 ENDS

LLVM_ROM_TAIL_18 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_18 ENDS

LLVM_ROM_BODY_19 SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_19 ENDS

LLVM_ROM_TAIL_19 SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_19 ENDS

LLVM_ROM_BODY_1A SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1A ENDS

LLVM_ROM_TAIL_1A SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1A ENDS

LLVM_ROM_BODY_1B SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1B ENDS

LLVM_ROM_TAIL_1B SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1B ENDS

LLVM_ROM_BODY_1C SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1C ENDS

LLVM_ROM_TAIL_1C SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1C ENDS

LLVM_ROM_BODY_1D SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1D ENDS

LLVM_ROM_TAIL_1D SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1D ENDS

LLVM_ROM_BODY_1E SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1E ENDS

LLVM_ROM_TAIL_1E SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1E ENDS

LLVM_ROM_BODY_1F SECTION CODE WORD PUBLIC 'LLVMROM'
        DS      0FFFEh
LLVM_ROM_BODY_1F ENDS

LLVM_ROM_TAIL_1F SECTION CODE WORD PUBLIC 'LLVMROM'
        DW      0000h
LLVM_ROM_TAIL_1F ENDS

LLVM_RAM_RESERVATION SECTION HDAT WORD PUBLIC 'LLVMRAM'
; Stable shared-data slots inside the fixed 2 MiB LLVM RAM overlay.  These
; labels are visible to TASKING assembly, while the matching ELF sections are
; overlaid at the same absolute addresses.
_llvm_float32_slot LABEL WORD
        PUBLIC  _llvm_float32_slot
        DS      004h
        DS      0FCh
_llvm_float_array LABEL WORD
        PUBLIC  _llvm_float_array
        DS      010h
_llvm_float_record LABEL WORD
        PUBLIC  _llvm_float_record
        DS      010h
        DS      0E0h
_llvm_double_array LABEL WORD
        PUBLIC  _llvm_double_array
        DS      020h
_llvm_double_record LABEL WORD
        PUBLIC  _llvm_double_record
        DS      018h
        DS      0C8h
_llvm_tasking_double_observed LABEL WORD
        PUBLIC  _llvm_tasking_double_observed
        DS      008h
; One page-aligned, sub-16-KiB object for every generated Classic _far
; pointer/indexing corpus.  Keeping it in the universal RAM reservation avoids
; per-case linker layouts and makes every pointer operation stay inside the
; object permitted by the TASKING Large model.
        DS      03CF8h
_seeded_far_arena LABEL WORD
        PUBLIC  _seeded_far_arena
        DS      03F00h
        DS      1F8100h
LLVM_RAM_RESERVATION ENDS

@IF( @TASKING_MODEL_IS_SMALL )
C166_US SECTION LDAT WORD GLBUSRSTACK 'CUSTACK'
@ELSE
C166_US SECTION DATA WORD GLBUSRSTACK 'CUSTACK'
@ENDI
; The page-1 stack shares DPP1/DPP2 so the exact 16 KiB boundary case
; can represent its one-past top.  0880h is the largest common reserve with a
; safe margin below the fixed 04900h floating arenas; per-object C166_US
; requirements extend or replace it up to the full Classic page limit.
        DS      00880h
C166_US ENDS

        REGDEF  R0-R15
        END
