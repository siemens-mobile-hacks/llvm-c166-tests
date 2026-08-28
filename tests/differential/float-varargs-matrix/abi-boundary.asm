$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT_VARARGS_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float_varargs_proxy1
        PUBLIC _llvm_float_varargs_proxy2
        PUBLIC _llvm_float_varargs_proxy3
        PUBLIC _llvm_float_varargs_proxy4
        PUBLIC _llvm_float_varargs_proxy5
        PUBLIC _llvm_float_promote_proxy

_llvm_float_varargs_proxy1 PROC FAR
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R12,_llvm_vararg_inputs
        MOV R13,_llvm_vararg_inputs+02h
        CALLS SEG _prepare_vararg_stack18,_prepare_vararg_stack18
        CALLS 10h,00100h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#04h
        RETS
_llvm_float_varargs_proxy1 ENDP

_llvm_float_varargs_proxy2 PROC FAR
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R12,_llvm_vararg_inputs
        MOV R13,_llvm_vararg_inputs+02h
        CALLS SEG _prepare_vararg_stack18,_prepare_vararg_stack18
        CALLS 10h,00500h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#04h
        RETS
_llvm_float_varargs_proxy2 ENDP

_llvm_float_varargs_proxy3 PROC FAR
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R12,_llvm_vararg_inputs
        MOV R13,_llvm_vararg_inputs+02h
        EXTP #PAG _llvm_vararg_inputs,#01h
        MOV R14,_llvm_vararg_inputs+04h
        CALLS SEG _prepare_vararg_stack18,_prepare_vararg_stack18
        CALLS 10h,00900h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#04h
        RETS
_llvm_float_varargs_proxy3 ENDP

_llvm_float_varargs_proxy4 PROC FAR
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R12,_llvm_vararg_inputs
        MOV R13,_llvm_vararg_inputs+02h
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R14,_llvm_vararg_inputs+04h
        MOV R15,_llvm_vararg_inputs+06h
        CALLS SEG _prepare_vararg_stack18,_prepare_vararg_stack18
        CALLS 10h,00D00h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#04h
        RETS
_llvm_float_varargs_proxy4 ENDP

_llvm_float_varargs_proxy5 PROC FAR
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R12,_llvm_vararg_inputs
        MOV R13,_llvm_vararg_inputs+02h
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R14,_llvm_vararg_inputs+04h
        MOV R15,_llvm_vararg_inputs+06h
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#06h
        EXTP #PAG _llvm_vararg_inputs,#01h
        MOV R1,_llvm_vararg_inputs+08h
        MOV [R0],R1
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+0Ah
        MOV R2,_llvm_vararg_inputs+0Ch
        MOV [R0+#02h],R1
        MOV [R0+#04h],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+0Eh
        MOV R2,_llvm_vararg_inputs+10h
        MOV [R0+#06h],R1
        MOV [R0+#08h],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+12h
        MOV R2,_llvm_vararg_inputs+14h
        MOV [R0+#0Ah],R1
        MOV [R0+#0Ch],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+16h
        MOV R2,_llvm_vararg_inputs+18h
        MOV [R0+#0Eh],R1
        MOV [R0+#10h],R2
        EXTP #PAG _llvm_vararg_inputs,#01h
        MOV R1,_llvm_vararg_inputs+1Ah
        MOV [R0+#12h],R1
        CALLS 10h,01100h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#06h
        RETS
_llvm_float_varargs_proxy5 ENDP

; shape remains in R12; the public float argument is stack-only and MSW-first.
_llvm_float_promote_proxy PROC FAR
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R14
        CALLS 10h,01500h
        ADD R0,#04h
        RETS
_llvm_float_promote_proxy ENDP

_prepare_vararg_stack18 PROC FAR
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#04h
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+0Ah
        MOV R2,_llvm_vararg_inputs+0Ch
        MOV [R0],R1
        MOV [R0+#02h],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+0Eh
        MOV R2,_llvm_vararg_inputs+10h
        MOV [R0+#04h],R1
        MOV [R0+#06h],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+12h
        MOV R2,_llvm_vararg_inputs+14h
        MOV [R0+#08h],R1
        MOV [R0+#0Ah],R2
        EXTP #PAG _llvm_vararg_inputs,#02h
        MOV R1,_llvm_vararg_inputs+16h
        MOV R2,_llvm_vararg_inputs+18h
        MOV [R0+#0Ch],R1
        MOV [R0+#0Eh],R2
        EXTP #PAG _llvm_vararg_inputs,#01h
        MOV R1,_llvm_vararg_inputs+1Ah
        MOV [R0+#10h],R1
        RETS
_prepare_vararg_stack18 ENDP
LLVM_PROXY_PR ENDS

FLOAT_VARARGS_VALUES SECTION DATA WORD PUBLIC 'FLOATVARGS'
_llvm_vararg_inputs LABEL WORD
        DS 01Ch
        PUBLIC _llvm_vararg_inputs
_llvm_vararg_captured_words LABEL WORD
        PUBLIC _llvm_vararg_captured_words
        DS 010h
_llvm_vararg_observed LABEL WORD
        DS 004h
        PUBLIC _llvm_vararg_observed
FLOAT_VARARGS_VALUES ENDS

        REGDEF R0-R15
        END
