$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_FAR_AUTOMATIC_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_stack_apply_proxy
        PUBLIC _llvm_stack_owner_local_proxy
        PUBLIC _llvm_stack_owner_tasking_proxy
        PUBLIC _llvm_stack_reverse_apply_proxy

_llvm_stack_apply_proxy PROC FAR
        PUSH R0
        CALLS 10h,0100h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,STACK_APPLY_R0_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
STACK_APPLY_R0_OK:
        RETS
_llvm_stack_apply_proxy ENDP

_llvm_stack_owner_local_proxy PROC FAR
        PUSH R0
        CALLS 10h,0900h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,STACK_OWNER_LOCAL_R0_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
STACK_OWNER_LOCAL_R0_OK:
        RETS
_llvm_stack_owner_local_proxy ENDP

_llvm_stack_owner_tasking_proxy PROC FAR
        PUSH R0
        CALLS 10h,0B00h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,STACK_OWNER_TASKING_R0_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
STACK_OWNER_TASKING_R0_OK:
        RETS
_llvm_stack_owner_tasking_proxy ENDP

_llvm_stack_reverse_apply_proxy PROC FAR
        PUSH R0
        CALLS 10h,0D00h
        POP R1
        CMP R1,R0
        JMPR cc_EQ,STACK_REVERSE_APPLY_R0_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
STACK_REVERSE_APPLY_R0_OK:
        RETS
_llvm_stack_reverse_apply_proxy ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
