$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME    LLVM_INDIRECT_MATRIX_ABI_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_call0_proxy
        PUBLIC _llvm_call2_proxy
        PUBLIC _llvm_call5_proxy
        PUBLIC _llvm_get0_proxy
        PUBLIC _llvm_get2_proxy
        PUBLIC _llvm_get5_proxy
        PUBLIC _llvm_canary0_proxy
        PUBLIC _llvm_canary2_proxy
        PUBLIC _llvm_canary5_proxy
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_call0_proxy PROC NEAR
@ELSE
_llvm_call0_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C500h
@ELSE
        CALLS 10h,0400h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_call0_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_call2_proxy PROC NEAR
@ELSE
_llvm_call2_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C700h
@ELSE
        CALLS 10h,0600h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_call2_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_call5_proxy PROC NEAR
@ELSE
_llvm_call5_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0CA00h
@ELSE
        CALLS 10h,0900h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_call5_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_get0_proxy PROC NEAR
@ELSE
_llvm_get0_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0CE00h
@ELSE
        CALLS 10h,0D00h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_get0_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_get2_proxy PROC NEAR
@ELSE
_llvm_get2_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0CE80h
@ELSE
        CALLS 10h,0D80h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_get2_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_get5_proxy PROC NEAR
@ELSE
_llvm_get5_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0CF00h
@ELSE
        CALLS 10h,0E00h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_get5_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_canary0_proxy PROC NEAR
@ELSE
_llvm_canary0_proxy PROC FAR
@ENDI
        PUSH R0
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C200h
@ELSE
        CALLS 10h,0100h
@ENDI
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY0_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY0_OK:
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_canary0_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_canary2_proxy PROC NEAR
@ELSE
_llvm_canary2_proxy PROC FAR
@ENDI
        PUSH R0
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C280h
@ELSE
        CALLS 10h,0180h
@ENDI
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY2_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY2_OK:
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_canary2_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_canary5_proxy PROC NEAR
@ELSE
_llvm_canary5_proxy PROC FAR
@ENDI
        PUSH R0
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C380h
@ELSE
        CALLS 10h,0280h
@ENDI
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY5_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY5_OK:
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_canary5_proxy ENDP

LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
