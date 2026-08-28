$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT_VOLATILE_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_volatile_f32_eval_proxy
        PUBLIC _llvm_volatile_f64_eval_proxy

_llvm_volatile_f32_eval_proxy PROC FAR
        CALLS 10h,00100h
        EXTP #PAG _volatile_observed,#02h
        MOV _volatile_observed,R4
        MOV _volatile_observed+02h,R5
        RETS
_llvm_volatile_f32_eval_proxy ENDP

_llvm_volatile_f64_eval_proxy PROC FAR
        SUB R0,#07h
        SUB R0,#01h
        CALLS 10h,00600h
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _volatile_observed,#02h
        MOV _volatile_observed,R10
        MOV _volatile_observed+02h,R11
        EXTP #PAG _volatile_observed,#02h
        MOV _volatile_observed+04h,R1
        MOV _volatile_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
        RETS
_llvm_volatile_f64_eval_proxy ENDP
LLVM_PROXY_PR ENDS

FLOAT_VOLATILE_VALUES SECTION DATA WORD PUBLIC 'FLOATVOL'
_volatile_f32_a LABEL WORD
        DS 010h
        PUBLIC _volatile_f32_a
_volatile_f32_b LABEL WORD
        DS 010h
        PUBLIC _volatile_f32_b
_volatile_f64_a LABEL WORD
        DS 020h
        PUBLIC _volatile_f64_a
_volatile_f64_b LABEL WORD
        DS 020h
        PUBLIC _volatile_f64_b
_volatile_observed LABEL WORD
        DS 008h
        PUBLIC _volatile_observed
        DS 018h
FLOAT_VOLATILE_VALUES ENDS

        REGDEF R0-R15
        END
