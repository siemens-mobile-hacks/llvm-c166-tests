$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT64_STORAGE_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float64_storage_eval_proxy

; TASKING calls this proxy as seven ordinary 16-bit arguments.  The first
; four arrive in R12-R15; w1, w0 and tail are on the incoming user stack.
; Rebuild the authoritative Classic call frame for
; (word, word, double, word): double and the trailing word are stack-only,
; MSW-first, followed by the caller-owned eight-byte result block.
_llvm_float64_storage_eval_proxy PROC FAR
        MOV R1,[R0]
        MOV R2,[R0+#02h]
        MOV R3,[R0+#04h]
        SUB R0,#07h
        SUB R0,#07h
        SUB R0,#04h
        MOV [R0],R14
        MOV [R0+#02h],R15
        MOV [R0+#04h],R1
        MOV [R0+#06h],R2
        MOV [R0+#08h],R3
        CALLS 10h,00100h
        MOV R10,[R4]
        MOV R11,[R4+#02h]
        MOV R1,[R4+#04h]
        MOV R2,[R4+#06h]
        EXTP #PAG _llvm_double_observed,#02h
        MOV _llvm_double_observed,R10
        MOV _llvm_double_observed+02h,R11
        EXTP #PAG _llvm_double_observed,#02h
        MOV _llvm_double_observed+04h,R1
        MOV _llvm_double_observed+06h,R2
        ADD R0,#07h
        ADD R0,#07h
        ADD R0,#04h
        RETS
_llvm_float64_storage_eval_proxy ENDP
LLVM_PROXY_PR ENDS

FLOAT64_STORAGE SECTION DATA WORD PUBLIC 'F64STORE'
_tasking_double_array LABEL WORD
        DS 020h
        PUBLIC _tasking_double_array
_tasking_double_record_src LABEL WORD
        DS 018h
        PUBLIC _tasking_double_record_src
        DS 008h
_tasking_double_record_dst LABEL WORD
        DS 018h
        PUBLIC _tasking_double_record_dst
        DS 008h
_llvm_double_observed LABEL WORD
        DS 008h
        PUBLIC _llvm_double_observed
_float64_storage_next_case LABEL WORD
        DS 002h
        PUBLIC _float64_storage_next_case
        DS 016h
FLOAT64_STORAGE ENDS

        REGDEF R0-R15
        END
