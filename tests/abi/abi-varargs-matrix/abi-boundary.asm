$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME    ABI_VARARGS_MATRIX_BOUNDARY

LLVM_PROXY_PR  SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC  _llvm_varargs_proxy1
        PUBLIC  _llvm_varargs_proxy2
        PUBLIC  _llvm_varargs_proxy3
        PUBLIC  _llvm_varargs_proxy4
        PUBLIC  _llvm_varargs_proxy5
        PUBLIC  _llvm_reverse_entry_proxy
        PUBLIC  _llvm_varargs_stream_proxy
        PUBLIC  _llvm_reverse_stream_entry_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_proxy1 PROC NEAR
@ELSE
_llvm_varargs_proxy1 PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C200h
@ELSE
        CALLS   10h,0100h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_proxy1 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_proxy2 PROC NEAR
@ELSE
_llvm_varargs_proxy2 PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0C600h
@ELSE
        CALLS   10h,0500h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_proxy2 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_proxy3 PROC NEAR
@ELSE
_llvm_varargs_proxy3 PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0CA00h
@ELSE
        CALLS   10h,0900h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_proxy3 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_proxy4 PROC NEAR
@ELSE
_llvm_varargs_proxy4 PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0CE00h
@ELSE
        CALLS   10h,0D00h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_proxy4 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_proxy5 PROC NEAR
@ELSE
_llvm_varargs_proxy5 PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0D200h
@ELSE
        CALLS   10h,1100h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_proxy5 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_reverse_entry_proxy PROC NEAR
@ELSE
_llvm_reverse_entry_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0DB00h
@ELSE
        CALLS   10h,1A00h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_reverse_entry_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_varargs_stream_proxy PROC NEAR
@ELSE
_llvm_varargs_stream_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0B400h
@ELSE
        CALLS   10h,2000h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_varargs_stream_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_reverse_stream_entry_proxy PROC NEAR
@ELSE
_llvm_reverse_stream_entry_proxy PROC FAR
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA   cc_UC,0E900h
@ELSE
        CALLS   10h,3000h
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_reverse_stream_entry_proxy ENDP

LLVM_PROXY_PR  ENDS

        REGDEF  R0-R15
        END
