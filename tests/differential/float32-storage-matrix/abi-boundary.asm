$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT32_STORAGE_ABI_BOUNDARY
        EXTERN  _llvm_float_array:WORD
        EXTERN  _llvm_float_record:WORD

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float_array_load_external_proxy
        PUBLIC _llvm_float_array_store_external_proxy
        PUBLIC _llvm_float_array_load_own_initial_proxy
        PUBLIC _llvm_float_array_load_own_proxy
        PUBLIC _llvm_float_array_store_own_proxy
        PUBLIC _llvm_float_record_load_external_proxy
        PUBLIC _llvm_float_record_store_external_proxy
        PUBLIC _llvm_float_record_load_own_initial_proxy
        PUBLIC _llvm_float_record_load_own_proxy
        PUBLIC _llvm_float_record_store_own_proxy
        PUBLIC _llvm_float_record_copy_external_proxy
        PUBLIC _llvm_float_record_local_proxy

; Seed a dynamically indexed TASKING-owned array element with Classic
; MSW:LSW physical words and expose LLVM's float return as unsigned long.
_llvm_float_array_load_external_proxy PROC FAR
        MOV R1,R14
        SHL R1,#02h
        MOV R2,#POF _tasking_float_array
        ADD R1,R2
        EXTP #PAG _tasking_float_array,#02h
        MOV [R1],R13
        MOV [R1+#02h],R12
        MOV R12,R14
        CALLS 10h,00100h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_array_load_external_proxy ENDP

_llvm_float_array_store_external_proxy PROC FAR
        PUSH R14
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        MOV R12,R14
        CALLS 10h,00200h
        ADD R0,#04h
        POP R1
        SHL R1,#02h
        MOV R2,#POF _tasking_float_array
        ADD R1,R2
        EXTP #PAG _tasking_float_array,#02h
        MOV R5,[R1]
        MOV R4,[R1+#02h]
        RETS
_llvm_float_array_store_external_proxy ENDP

_llvm_float_array_load_own_initial_proxy PROC FAR
        CALLS 10h,00300h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_array_load_own_initial_proxy ENDP

_llvm_float_array_load_own_proxy PROC FAR
        MOV R1,R14
        SHL R1,#02h
        MOV R2,#POF _llvm_float_array
        ADD R1,R2
        EXTP #PAG _llvm_float_array,#02h
        MOV [R1],R13
        MOV [R1+#02h],R12
        MOV R12,R14
        CALLS 10h,00300h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_array_load_own_proxy ENDP

_llvm_float_array_store_own_proxy PROC FAR
        PUSH R14
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        MOV R12,R14
        CALLS 10h,00400h
        ADD R0,#04h
        POP R1
        SHL R1,#02h
        MOV R2,#POF _llvm_float_array
        ADD R1,R2
        EXTP #PAG _llvm_float_array,#02h
        MOV R5,[R1]
        MOV R4,[R1+#02h]
        RETS
_llvm_float_array_store_own_proxy ENDP

_llvm_float_record_load_external_proxy PROC FAR
        CMP R14,#00h
        JMPR cc_NE,RECORD_LOAD_EXTERNAL_SECOND
        EXTP #PAG _tasking_float_record_src,#02h
        MOV _tasking_float_record_src+02h,R13
        MOV _tasking_float_record_src+04h,R12
        JMPR cc_UC,RECORD_LOAD_EXTERNAL_CALL
RECORD_LOAD_EXTERNAL_SECOND:
        EXTP #PAG _tasking_float_record_src,#02h
        MOV _tasking_float_record_src+08h,R13
        MOV _tasking_float_record_src+0Ah,R12
RECORD_LOAD_EXTERNAL_CALL:
        MOV R12,R14
        CALLS 10h,00500h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_record_load_external_proxy ENDP

_llvm_float_record_store_external_proxy PROC FAR
        PUSH R14
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        MOV R12,R14
        CALLS 10h,00600h
        ADD R0,#04h
        POP R1
        CMP R1,#00h
        JMPR cc_NE,RECORD_STORE_EXTERNAL_SECOND
        EXTP #PAG _tasking_float_record_src,#02h
        MOV R5,_tasking_float_record_src+02h
        MOV R4,_tasking_float_record_src+04h
        RETS
RECORD_STORE_EXTERNAL_SECOND:
        EXTP #PAG _tasking_float_record_src,#02h
        MOV R5,_tasking_float_record_src+08h
        MOV R4,_tasking_float_record_src+0Ah
        RETS
_llvm_float_record_store_external_proxy ENDP

_llvm_float_record_load_own_initial_proxy PROC FAR
        CALLS 10h,00700h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_record_load_own_initial_proxy ENDP

_llvm_float_record_load_own_proxy PROC FAR
        CMP R14,#00h
        JMPR cc_NE,RECORD_LOAD_OWN_SECOND
        EXTP #PAG _llvm_float_record,#02h
        MOV POF (_llvm_float_record+02h),R13
        MOV POF (_llvm_float_record+04h),R12
        JMPR cc_UC,RECORD_LOAD_OWN_CALL
RECORD_LOAD_OWN_SECOND:
        EXTP #PAG _llvm_float_record,#02h
        MOV POF (_llvm_float_record+08h),R13
        MOV POF (_llvm_float_record+0Ah),R12
RECORD_LOAD_OWN_CALL:
        MOV R12,R14
        CALLS 10h,00700h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_record_load_own_proxy ENDP

_llvm_float_record_store_own_proxy PROC FAR
        PUSH R14
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        MOV R12,R14
        CALLS 10h,00800h
        ADD R0,#04h
        POP R1
        CMP R1,#00h
        JMPR cc_NE,RECORD_STORE_OWN_SECOND
        EXTP #PAG _llvm_float_record,#02h
        MOV R5,POF (_llvm_float_record+02h)
        MOV R4,POF (_llvm_float_record+04h)
        RETS
RECORD_STORE_OWN_SECOND:
        EXTP #PAG _llvm_float_record,#02h
        MOV R5,POF (_llvm_float_record+08h)
        MOV R4,POF (_llvm_float_record+0Ah)
        RETS
_llvm_float_record_store_own_proxy ENDP

_llvm_float_record_copy_external_proxy PROC FAR
        MOV R1,#01357h
        MOV R2,#02468h
        MOV R3,#0CDEFh
        MOV R4,#089ABh
        EXTP #PAG _tasking_float_record_src,#04h
        MOV _tasking_float_record_src,R1
        MOV _tasking_float_record_src+02h,R13
        MOV _tasking_float_record_src+04h,R12
        MOV _tasking_float_record_src+06h,R2
        EXTP #PAG _tasking_float_record_src,#04h
        MOV _tasking_float_record_src+08h,R13
        MOV _tasking_float_record_src+0Ah,R12
        MOV _tasking_float_record_src+0Ch,R3
        MOV _tasking_float_record_src+0Eh,R4
        MOV R12,R14
        CALLS 10h,00900h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_record_copy_external_proxy ENDP

_llvm_float_record_local_proxy PROC FAR
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        MOV R12,R14
        CALLS 10h,00B00h
        ADD R0,#04h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float_record_local_proxy ENDP
LLVM_PROXY_PR ENDS

TASKING_FLOAT_STORAGE SECTION DATA WORD PUBLIC 'FLOATSTORE'
_tasking_float_array LABEL WORD
        DS 010h
        PUBLIC _tasking_float_array
_tasking_float_record_src LABEL WORD
        DS 010h
        PUBLIC _tasking_float_record_src
_tasking_float_record_dst LABEL WORD
        DS 010h
        PUBLIC _tasking_float_record_dst
        DS 010h
TASKING_FLOAT_STORAGE ENDS

        REGDEF R0-R15
        END
