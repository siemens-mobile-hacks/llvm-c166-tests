$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK
; simulator.sh supplies MODEL(...) and TASKING_MODEL_IS_MEDIUM.

        NAME    SEEDED_F32_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_seeded_f32_eval_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_seeded_f32_eval_proxy PROC NEAR
        CALLA cc_UC,0C100h
@ELSE
_llvm_seeded_f32_eval_proxy PROC FAR
        CALLS 10h,00100h
@ENDI
        ; LLVM returns Classic float as R4=MSW,R5=LSW.  Expose raw bits as
        ; TASKING unsigned long R4=LSW,R5=MSW.
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_seeded_f32_eval_proxy ENDP
LLVM_PROXY_PR ENDS

@IF( @TASKING_MODEL_IS_SMALL )
SEEDED_F32_VALUES SECTION LDAT WORD PUBLIC 'SEEDEDF32'
@ELSE
SEEDED_F32_VALUES SECTION DATA WORD PUBLIC 'SEEDEDF32'
@ENDI
_seeded_f32_inputs LABEL WORD
        DS 008h
        PUBLIC _seeded_f32_inputs
        DS 018h
SEEDED_F32_VALUES ENDS

        REGDEF R0-R15
        END
