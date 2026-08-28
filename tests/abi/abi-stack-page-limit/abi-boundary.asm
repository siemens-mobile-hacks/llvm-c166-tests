$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_STACK_PAGE_LIMIT_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _tasking_stack_page_limit_proxy
        PUBLIC _llvm_stack_page_limit_proxy
        PUBLIC _tasking_calls_llvm_stack_page_limit_proxy
        PUBLIC _llvm_calls_tasking_stack_page_limit_proxy
        PUBLIC _llvm_stack_escape_store_proxy
        PUBLIC _asm_stack_escape_llvm_proxy
        PUBLIC _asm_stack_escape_tasking_proxy
        PUBLIC _llvm_stack_page_limit_bridge
        PUBLIC _llvm_stack_edge_apply_bridge

_llvm_stack_page_limit_bridge PROC FAR
        CALLS 10h,0100h
        RETS
_llvm_stack_page_limit_bridge ENDP

_llvm_stack_edge_apply_bridge PROC FAR
        CMP R12,#03FFEh
        JMPR cc_NE,STACK_EDGE_BAD_OFFSET
        CMP R13,#01h
        JMPR cc_NE,STACK_EDGE_BAD_PAGE
        CALLS 10h,0B00h
        RETS
STACK_EDGE_BAD_OFFSET:
        MOV R4,#0E001h
        RETS
STACK_EDGE_BAD_PAGE:
        MOV R4,#0E002h
        RETS
_llvm_stack_edge_apply_bridge ENDP

_tasking_stack_page_limit_proxy PROC FAR
        MOV R2,#00h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_tasking_stack_page_limit_proxy ENDP

_llvm_stack_page_limit_proxy PROC FAR
        MOV R2,#01h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_llvm_stack_page_limit_proxy ENDP

_tasking_calls_llvm_stack_page_limit_proxy PROC FAR
        MOV R2,#02h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_tasking_calls_llvm_stack_page_limit_proxy ENDP

_llvm_calls_tasking_stack_page_limit_proxy PROC FAR
        MOV R2,#03h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_llvm_calls_tasking_stack_page_limit_proxy ENDP

_llvm_stack_escape_store_proxy PROC FAR
        MOV R2,#04h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        MOV R12,#03300h
        MOV R13,#03h
        CALLS SEG CHECK_ESCAPED_POINTER,CHECK_ESCAPED_POINTER
        RETS
_llvm_stack_escape_store_proxy ENDP

_asm_stack_escape_llvm_proxy PROC FAR
        MOV R2,#05h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_asm_stack_escape_llvm_proxy ENDP

_asm_stack_escape_tasking_proxy PROC FAR
        MOV R2,#06h
        CALLS SEG PAGE_LIMIT_CALL,PAGE_LIMIT_CALL
        RETS
_asm_stack_escape_tasking_proxy ENDP

PAGE_LIMIT_CALL PROC FAR
        PUSH R0
        MOV R0,#08000h
        CMP R2,#00h
        JMPR cc_EQ,PAGE_LIMIT_TASKING
        CMP R2,#01h
        JMPR cc_EQ,PAGE_LIMIT_LLVM
        CMP R2,#02h
        JMPR cc_EQ,PAGE_LIMIT_TASKING_LLVM
        CMP R2,#03h
        JMPR cc_EQ,PAGE_LIMIT_LLVM_TASKING
        CMP R2,#04h
        JMPR cc_EQ,PAGE_LIMIT_LLVM_ESCAPE_LOCAL
        CMP R2,#05h
        JMPR cc_EQ,PAGE_LIMIT_TASKING_ESCAPE_LLVM
        JMPR cc_UC,PAGE_LIMIT_TASKING_ESCAPE_TASKING
PAGE_LIMIT_TASKING:
        CALLS 09h,08000h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_LLVM:
        CALLS 10h,0100h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_TASKING_LLVM:
        CALLS 09h,09000h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_LLVM_TASKING:
        CALLS 10h,0900h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_LLVM_ESCAPE_LOCAL:
        CALLS 10h,0D00h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_TASKING_ESCAPE_LLVM:
        MOV R0,#07000h
        MOV R12,#03FFEh
        MOV R13,#01h
        MOV R14,#04567h
        CMP R12,#03FFEh
        JMPR cc_NE,PAGE_LIMIT_BAD_CALL_OFFSET
        CMP R13,#01h
        JMPR cc_NE,PAGE_LIMIT_BAD_CALL_PAGE
        CALLS 10h,0B00h
        MOV R0,#08000h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_TASKING_ESCAPE_TASKING:
        MOV R0,#07000h
        MOV R12,#03FFEh
        MOV R13,#01h
        MOV R14,#05678h
        CMP R12,#03FFEh
        JMPR cc_NE,PAGE_LIMIT_BAD_CALL_OFFSET
        CMP R13,#01h
        JMPR cc_NE,PAGE_LIMIT_BAD_CALL_PAGE
        CALLS 09h,0A000h
        MOV R0,#08000h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_BAD_CALL_OFFSET:
        MOV R4,#0E004h
        JMPR cc_UC,PAGE_LIMIT_DONE
PAGE_LIMIT_BAD_CALL_PAGE:
        MOV R4,#0E005h
PAGE_LIMIT_DONE:
        CMP R0,#08000h
        JMPR cc_EQ,PAGE_LIMIT_R0_OK
        MOV R4,#0C0DEh
PAGE_LIMIT_R0_OK:
        POP R0
        RETS
PAGE_LIMIT_CALL ENDP

CHECK_ESCAPED_POINTER PROC FAR
        EXTP R13,#01h
        MOV R1,[R12]
        CMP R1,#03FFEh
        JMPR cc_NE,CHECK_ESCAPED_BAD_OFFSET
        ADD R12,#02h
        EXTP R13,#01h
        MOV R1,[R12]
        CMP R1,#01h
        JMPR cc_NE,CHECK_ESCAPED_BAD_PAGE
        RETS
CHECK_ESCAPED_BAD_OFFSET:
        MOV R4,#0E001h
        RETS
CHECK_ESCAPED_BAD_PAGE:
        MOV R4,#0E002h
        RETS
CHECK_ESCAPED_POINTER ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
