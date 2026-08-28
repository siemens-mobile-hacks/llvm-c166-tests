$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK
; simulator.sh supplies MODEL(...) and TASKING_MODEL_IS_MEDIUM.

        NAME    SEEDED_F64_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_seeded_f64_eval_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_seeded_f64_eval_proxy PROC NEAR
@ELSE
_llvm_seeded_f64_eval_proxy PROC FAR
@ENDI
        SUB R0,#07h
        SUB R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C100h
@ELSE
        CALLS 10h,00100h
@ENDI
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _seeded_f64_observed,#02h
        MOV _seeded_f64_observed,R10
        MOV _seeded_f64_observed+02h,R11
        EXTP #PAG _seeded_f64_observed,#02h
        MOV _seeded_f64_observed+04h,R1
        MOV _seeded_f64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_seeded_f64_eval_proxy ENDP
LLVM_PROXY_PR ENDS

@IF( @TASKING_MODEL_IS_SMALL )
SEEDED_F64_VALUES SECTION LDAT WORD PUBLIC 'SEEDEDF64'
@ELSE
SEEDED_F64_VALUES SECTION DATA WORD PUBLIC 'SEEDEDF64'
@ENDI
_seeded_f64_inputs LABEL WORD
        DS 018h
        PUBLIC _seeded_f64_inputs
_seeded_f64_observed LABEL WORD
        DS 008h
        PUBLIC _seeded_f64_observed
        DS 020h
SEEDED_F64_VALUES ENDS

        REGDEF R0-R15
        END
