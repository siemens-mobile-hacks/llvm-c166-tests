$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

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
_llvm_call0_proxy PROC FAR
        CALLS 10h,0400h
        RETS
_llvm_call0_proxy ENDP
_llvm_call2_proxy PROC FAR
        CALLS 10h,0600h
        RETS
_llvm_call2_proxy ENDP
_llvm_call5_proxy PROC FAR
        CALLS 10h,0900h
        RETS
_llvm_call5_proxy ENDP

_llvm_get0_proxy PROC FAR
        CALLS 10h,0D00h
        RETS
_llvm_get0_proxy ENDP
_llvm_get2_proxy PROC FAR
        CALLS 10h,0D80h
        RETS
_llvm_get2_proxy ENDP
_llvm_get5_proxy PROC FAR
        CALLS 10h,0E00h
        RETS
_llvm_get5_proxy ENDP

_llvm_canary0_proxy PROC FAR
        PUSH R0
        CALLS 10h,0100h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY0_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY0_OK:
        RETS
_llvm_canary0_proxy ENDP

_llvm_canary2_proxy PROC FAR
        PUSH R0
        CALLS 10h,0180h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY2_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY2_OK:
        RETS
_llvm_canary2_proxy ENDP

_llvm_canary5_proxy PROC FAR
        PUSH R0
        CALLS 10h,0280h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,CANARY5_OK
        MOV R4,#0DEADh
        MOV R5,#0DEADh
CANARY5_OK:
        RETS
_llvm_canary5_proxy ENDP

LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
