$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

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

_llvm_varargs_proxy1 PROC FAR
        CALLS   10h,0100h
        RETS
_llvm_varargs_proxy1 ENDP

_llvm_varargs_proxy2 PROC FAR
        CALLS   10h,0500h
        RETS
_llvm_varargs_proxy2 ENDP

_llvm_varargs_proxy3 PROC FAR
        CALLS   10h,0900h
        RETS
_llvm_varargs_proxy3 ENDP

_llvm_varargs_proxy4 PROC FAR
        CALLS   10h,0D00h
        RETS
_llvm_varargs_proxy4 ENDP

_llvm_varargs_proxy5 PROC FAR
        CALLS   10h,1100h
        RETS
_llvm_varargs_proxy5 ENDP

_llvm_reverse_entry_proxy PROC FAR
        CALLS   10h,1A00h
        RETS
_llvm_reverse_entry_proxy ENDP

_llvm_varargs_stream_proxy PROC FAR
        CALLS   10h,2000h
        RETS
_llvm_varargs_stream_proxy ENDP

_llvm_reverse_stream_entry_proxy PROC FAR
        CALLS   10h,2800h
        RETS
_llvm_reverse_stream_entry_proxy ENDP

LLVM_PROXY_PR  ENDS

        REGDEF  R0-R15
        END
