$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME FLOAT_VARARGS_ABI_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float_varargs_proxy1
        PUBLIC _llvm_float_varargs_proxy2
        PUBLIC _llvm_float_varargs_proxy3
        PUBLIC _llvm_float_varargs_proxy4
        PUBLIC _llvm_float_varargs_proxy5
        PUBLIC _llvm_float_promote_proxy
        PUBLIC _llvm_float_raw_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_varargs_proxy1 PROC NEAR
        CALLA cc_UC,0B400h
        RET
@ELSE
_llvm_float_varargs_proxy1 PROC FAR
        CALLS 10h,00100h
        RETS
@ENDI
_llvm_float_varargs_proxy1 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_varargs_proxy2 PROC NEAR
        CALLA cc_UC,0B800h
        RET
@ELSE
_llvm_float_varargs_proxy2 PROC FAR
        CALLS 10h,00500h
        RETS
@ENDI
_llvm_float_varargs_proxy2 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_varargs_proxy3 PROC NEAR
        CALLA cc_UC,0BC00h
        RET
@ELSE
_llvm_float_varargs_proxy3 PROC FAR
        CALLS 10h,00900h
        RETS
@ENDI
_llvm_float_varargs_proxy3 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_varargs_proxy4 PROC NEAR
        CALLA cc_UC,0C000h
        RET
@ELSE
_llvm_float_varargs_proxy4 PROC FAR
        CALLS 10h,00D00h
        RETS
@ENDI
_llvm_float_varargs_proxy4 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_varargs_proxy5 PROC NEAR
        CALLA cc_UC,0C400h
        RET
@ELSE
_llvm_float_varargs_proxy5 PROC FAR
        CALLS 10h,01100h
        RETS
@ENDI
_llvm_float_varargs_proxy5 ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_promote_proxy PROC NEAR
        CALLA cc_UC,0C800h
        RET
@ELSE
_llvm_float_promote_proxy PROC FAR
        CALLS 10h,01500h
        RETS
@ENDI
_llvm_float_promote_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_float_raw_proxy PROC NEAR
        CALLA cc_UC,0D800h
        RET
@ELSE
_llvm_float_raw_proxy PROC FAR
        CALLS 10h,02000h
        RETS
@ENDI
_llvm_float_raw_proxy ENDP
LLVM_PROXY_PR ENDS

FLOAT_VARARGS_VALUES SECTION DATA WORD PUBLIC 'FLOATVARGS'
_vararg_captured LABEL WORD
_vararg_captured_words LABEL WORD
        PUBLIC _vararg_captured
        PUBLIC _vararg_captured_words
        DS 010h
_vararg_observed LABEL WORD
        PUBLIC _vararg_observed
        DS 004h
FLOAT_VARARGS_VALUES ENDS

        REGDEF R0-R15
        END
