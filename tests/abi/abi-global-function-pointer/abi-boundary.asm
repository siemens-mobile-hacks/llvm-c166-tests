$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
; simulator.sh supplies MODEL(...) and TASKING_MODEL_IS_MEDIUM.

        NAME    ABI_GLOBAL_FUNCTION_POINTER_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_global_exchange_proxy
        PUBLIC _llvm_call_global_proxy
        PUBLIC _llvm_global_roundtrip_proxy
        PUBLIC _llvm_get_target_proxy
        PUBLIC _llvm_reverse_roundtrip_proxy
        PUBLIC _llvm_reverse_exchange_proxy
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_global_exchange_proxy PROC NEAR
        CALLA cc_UC,0C180h
        RET
@ELSE
_llvm_global_exchange_proxy PROC FAR
        CALLS 10h,0180h
        RETS
@ENDI
_llvm_global_exchange_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_call_global_proxy PROC NEAR
        CALLA cc_UC,0C200h
        RET
@ELSE
_llvm_call_global_proxy PROC FAR
        CALLS 10h,0200h
        RETS
@ENDI
_llvm_call_global_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_global_roundtrip_proxy PROC NEAR
        CALLA cc_UC,0C280h
        RET
@ELSE
_llvm_global_roundtrip_proxy PROC FAR
        CALLS 10h,0280h
        RETS
@ENDI
_llvm_global_roundtrip_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_get_target_proxy PROC NEAR
        CALLA cc_UC,0C480h
        RET
@ELSE
_llvm_get_target_proxy PROC FAR
        CALLS 10h,0480h
        RETS
@ENDI
_llvm_get_target_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_reverse_roundtrip_proxy PROC NEAR
        CALLA cc_UC,0C500h
        RET
@ELSE
_llvm_reverse_roundtrip_proxy PROC FAR
        CALLS 10h,0500h
        RETS
@ENDI
_llvm_reverse_roundtrip_proxy ENDP
@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_reverse_exchange_proxy PROC NEAR
        CALLA cc_UC,0C580h
        RET
@ELSE
_llvm_reverse_exchange_proxy PROC FAR
        CALLS 10h,0580h
        RETS
@ENDI
_llvm_reverse_exchange_proxy ENDP

LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
