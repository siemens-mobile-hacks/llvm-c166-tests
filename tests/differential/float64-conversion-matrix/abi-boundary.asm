$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME    FLOAT64_CONVERSION_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_f64_to_i32_proxy
        PUBLIC _llvm_f64_to_u32_proxy
        PUBLIC _llvm_f64_to_i16_proxy
        PUBLIC _llvm_f64_to_u16_proxy
        PUBLIC _llvm_i32_to_f64_proxy
        PUBLIC _llvm_u32_to_f64_proxy
        PUBLIC _llvm_i16_to_f64_proxy
        PUBLIC _llvm_u16_to_f64_proxy
        PUBLIC _llvm_f64_compare_proxy

; TASKING supplies four raw words in R12-R15.  Build the public stack-only,
; MSW-first double argument without depending on its unavailable Large
; floating runtime.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_to_i32_proxy PROC NEAR
@ELSE
_llvm_f64_to_i32_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
        MOV [R0],R12
        MOV [R0+#02h],R13
        MOV [R0+#04h],R14
        MOV [R0+#06h],R15
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0B100h
@ELSE
        CALLS 10h,00100h
@ENDI
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_to_i32_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_to_u32_proxy PROC NEAR
@ELSE
_llvm_f64_to_u32_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
        MOV [R0],R12
        MOV [R0+#02h],R13
        MOV [R0+#04h],R14
        MOV [R0+#06h],R15
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0B300h
@ELSE
        CALLS 10h,00300h
@ENDI
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_to_u32_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_to_i16_proxy PROC NEAR
@ELSE
_llvm_f64_to_i16_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
        MOV [R0],R12
        MOV [R0+#02h],R13
        MOV [R0+#04h],R14
        MOV [R0+#06h],R15
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0B500h
@ELSE
        CALLS 10h,00500h
@ENDI
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_to_i16_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_to_u16_proxy PROC NEAR
@ELSE
_llvm_f64_to_u16_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
        MOV [R0],R12
        MOV [R0+#02h],R13
        MOV [R0+#04h],R14
        MOV [R0+#06h],R15
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0B700h
@ELSE
        CALLS 10h,00700h
@ENDI
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_to_u16_proxy ENDP

; Integer inputs remain in their ordinary Classic registers.  Allocate the
; caller-owned double result, invoke LLVM, and expose its four physical words
; without requiring TASKING's unavailable Large floating runtime.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_i32_to_f64_proxy PROC NEAR
@ELSE
_llvm_i32_to_f64_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0B900h
@ELSE
        CALLS 10h,00900h
@ENDI
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed,R10
        MOV _llvm_f64_observed+02h,R11
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed+04h,R1
        MOV _llvm_f64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_i32_to_f64_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_u32_to_f64_proxy PROC NEAR
@ELSE
_llvm_u32_to_f64_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0BB00h
@ELSE
        CALLS 10h,00B00h
@ENDI
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed,R10
        MOV _llvm_f64_observed+02h,R11
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed+04h,R1
        MOV _llvm_f64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_u32_to_f64_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_i16_to_f64_proxy PROC NEAR
@ELSE
_llvm_i16_to_f64_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0BD00h
@ELSE
        CALLS 10h,00D00h
@ENDI
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed,R10
        MOV _llvm_f64_observed+02h,R11
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed+04h,R1
        MOV _llvm_f64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_i16_to_f64_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_u16_to_f64_proxy PROC NEAR
@ELSE
_llvm_u16_to_f64_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0BF00h
@ELSE
        CALLS 10h,00F00h
@ENDI
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed,R10
        MOV _llvm_f64_observed+02h,R11
        EXTP #PAG _llvm_f64_observed,#02h
        MOV _llvm_f64_observed+04h,R1
        MOV _llvm_f64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_u16_to_f64_proxy ENDP

; TASKING signature is nine ordinary words.  Operation is R12; lhs[3:1] are
; R13-R15; lhs[0] and rhs[3:0] are on the incoming stack.  Rebuild the public
; (word, double, double) frame with both doubles stack-only and MSW-first.
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_f64_compare_proxy PROC NEAR
@ELSE
_llvm_f64_compare_proxy PROC FAR
@ENDI
        MOV R1,[R0]
        MOV R2,[R0+#02h]
        MOV R3,[R0+#04h]
        MOV R4,[R0+#06h]
        MOV R5,[R0+#08h]
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#02h
        MOV [R0],R13
        MOV [R0+#02h],R14
        MOV [R0+#04h],R15
        MOV [R0+#06h],R1
        MOV [R0+#08h],R2
        MOV [R0+#0Ah],R3
        MOV [R0+#0Ch],R4
        MOV [R0+#0Eh],R5
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C100h
@ELSE
        CALLS 10h,01100h
@ENDI
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#02h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_f64_compare_proxy ENDP
LLVM_PROXY_PR ENDS

@IF( @TASKING_MODEL_IS_SMALL )
TASKING_DOUBLE_VALUES SECTION LDAT WORD PUBLIC 'F64VALUES'
@ELSE
TASKING_DOUBLE_VALUES SECTION DATA WORD PUBLIC 'F64VALUES'
@ENDI
_llvm_f64_observed LABEL WORD
        DS 008h
        PUBLIC _llvm_f64_observed
TASKING_DOUBLE_VALUES ENDS

        REGDEF R0-R15
        END
