$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME ABI_FP_CALLS_BOUNDARY
LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_fp_float_bridge
        PUBLIC _llvm_fp_double_bridge
        PUBLIC _llvm_fp_reverse_bridge

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_fp_float_bridge PROC NEAR
        CALLA cc_UC,0B400h
        RET
@ELSE
_llvm_fp_float_bridge PROC FAR
        CALLS 10h,0100h
        RETS
@ENDI
_llvm_fp_float_bridge ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_fp_double_bridge PROC NEAR
        CALLA cc_UC,0BC00h
        RET
@ELSE
_llvm_fp_double_bridge PROC FAR
        CALLS 10h,0800h
        RETS
@ENDI
_llvm_fp_double_bridge ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_fp_reverse_bridge PROC NEAR
        CALLA cc_UC,0C400h
        RET
@ELSE
_llvm_fp_reverse_bridge PROC FAR
        CALLS 10h,1000h
        RETS
@ENDI
_llvm_fp_reverse_bridge ENDP
LLVM_PROXY_PR ENDS
        REGDEF R0-R15
        END
