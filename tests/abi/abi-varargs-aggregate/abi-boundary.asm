$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME ABI_VARARGS_AGGREGATE_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_varargs_aggregate_call_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_aggregate_call_proxy PROC NEAR
        CALLA cc_UC,0C100h
        RET
@ELSE
_llvm_varargs_aggregate_call_proxy PROC FAR
        CALLS 10h,0100h
        RETS
@ENDI
_llvm_varargs_aggregate_call_proxy ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
