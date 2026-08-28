$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_LONG_LONG_BOUNDARY

LLVM_PROXY_PR  SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC  _llvm_long_long_record_proxy

_llvm_long_long_record_proxy PROC FAR
        CALLS   10h,0800h
        RETS
_llvm_long_long_record_proxy ENDP

LLVM_PROXY_PR  ENDS

        REGDEF  R0-R15
        END
