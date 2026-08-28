$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT64_ARITHMETIC_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float64_eval_proxy

; TASKING supplies operation/lhs[0..2] in R12-R15 and the remaining five
; words at the incoming user stack.  Rebuild the authoritative Classic call
; frame for (word, double, double, word): both doubles and the tail are
; stack-only, MSW-first, followed by the caller-owned eight-byte result block.
_llvm_float64_eval_proxy PROC FAR
        MOV R1,[R0]
        MOV R2,[R0+#02h]
        MOV R3,[R0+#04h]
        MOV R4,[R0+#06h]
        MOV R5,[R0+#08h]
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#05h
        MOV [R0],R13
        MOV [R0+#02h],R14
        MOV [R0+#04h],R15
        MOV [R0+#06h],R1
        MOV [R0+#08h],R2
        MOV [R0+#0Ah],R3
        MOV [R0+#0Ch],R4
        MOV [R0+#0Eh],R5
        MOV R1,#05A5Ah
        MOV [R0+#10h],R1
        CALLS 10h,00100h
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_float64_observed,#02h
        MOV _llvm_float64_observed,R10
        MOV _llvm_float64_observed+02h,R11
        EXTP #PAG _llvm_float64_observed,#02h
        MOV _llvm_float64_observed+04h,R1
        MOV _llvm_float64_observed+06h,R2
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#05h
        RETS
_llvm_float64_eval_proxy ENDP
LLVM_PROXY_PR ENDS

FLOAT64_OBSERVED SECTION DATA WORD PUBLIC 'FLOAT64OBS'
_llvm_float64_observed LABEL WORD
        DS 08h
        PUBLIC _llvm_float64_observed
FLOAT64_OBSERVED ENDS

        REGDEF R0-R15
        END
