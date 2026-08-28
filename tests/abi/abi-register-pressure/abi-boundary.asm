$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_REGISTER_PRESSURE_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _tasking_pressure_words_state_proxy
        PUBLIC _llvm_pressure_words_state_proxy
        PUBLIC _llvm_pressure_words_tasking_leaf_state_proxy
        PUBLIC _tasking_pressure_words_llvm_leaf_state_proxy
        PUBLIC _llvm_call_tasking_pressure_words_state_proxy
        PUBLIC _tasking_pressure_longs_state_proxy
        PUBLIC _llvm_pressure_longs_state_proxy
        PUBLIC _llvm_pressure_longs_tasking_leaf_state_proxy
        PUBLIC _tasking_pressure_longs_llvm_leaf_state_proxy
        PUBLIC _llvm_call_tasking_pressure_longs_state_proxy
        PUBLIC _llvm_pressure_selector_bridge

_llvm_pressure_selector_bridge PROC FAR
        CALLS 10h,02100h
        RETS
_llvm_pressure_selector_bridge ENDP

_tasking_pressure_words_state_proxy PROC FAR
        MOV R2,#00h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_tasking_pressure_words_state_proxy ENDP
_llvm_pressure_words_state_proxy PROC FAR
        MOV R2,#01h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_pressure_words_state_proxy ENDP
_llvm_pressure_words_tasking_leaf_state_proxy PROC FAR
        MOV R2,#02h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_pressure_words_tasking_leaf_state_proxy ENDP
_tasking_pressure_words_llvm_leaf_state_proxy PROC FAR
        MOV R2,#03h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_tasking_pressure_words_llvm_leaf_state_proxy ENDP
_llvm_call_tasking_pressure_words_state_proxy PROC FAR
        MOV R2,#04h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_call_tasking_pressure_words_state_proxy ENDP

_tasking_pressure_longs_state_proxy PROC FAR
        MOV R2,#05h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_tasking_pressure_longs_state_proxy ENDP
_llvm_pressure_longs_state_proxy PROC FAR
        MOV R2,#06h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_pressure_longs_state_proxy ENDP
_llvm_pressure_longs_tasking_leaf_state_proxy PROC FAR
        MOV R2,#07h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_pressure_longs_tasking_leaf_state_proxy ENDP
_tasking_pressure_longs_llvm_leaf_state_proxy PROC FAR
        MOV R2,#08h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_tasking_pressure_longs_llvm_leaf_state_proxy ENDP
_llvm_call_tasking_pressure_longs_state_proxy PROC FAR
        MOV R2,#09h
        CALLS SEG PRESSURE_STATE_CALL,PRESSURE_STATE_CALL
        RETS
_llvm_call_tasking_pressure_longs_state_proxy ENDP

PRESSURE_STATE_CALL PROC FAR
        MOV R1,SP
        PUSH R1
        PUSH R0
        PUSH R6
        PUSH R7
        PUSH R8
        PUSH R9
        MOV R1,DPP0
        PUSH R1
        MOV R1,DPP1
        PUSH R1
        MOV R1,DPP2
        PUSH R1
        MOV R1,DPP3
        PUSH R1

        CMP R2,#00h
        JMPR cc_EQ,PRESSURE_CALL_TASKING_WORD
        CMP R2,#01h
        JMPR cc_EQ,PRESSURE_CALL_LLVM_WORD
        CMP R2,#02h
        JMPR cc_EQ,PRESSURE_CALL_LLVM_WORD_TASKING
        CMP R2,#03h
        JMPR cc_EQ,PRESSURE_CALL_TASKING_WORD_LLVM
        CMP R2,#04h
        JMPR cc_EQ,PRESSURE_CALL_LLVM_TASKING_WORD
        CMP R2,#05h
        JMPR cc_EQ,PRESSURE_CALL_TASKING_LONG
        CMP R2,#06h
        JMPR cc_EQ,PRESSURE_CALL_LLVM_LONG
        CMP R2,#07h
        JMPR cc_EQ,PRESSURE_CALL_LLVM_LONG_TASKING
        CMP R2,#08h
        JMPR cc_EQ,PRESSURE_CALL_TASKING_LONG_LLVM
        CALLS 10h,02900h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_TASKING_WORD:
        CALLS 09h,09000h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_LLVM_WORD:
        CALLS 10h,00100h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_LLVM_WORD_TASKING:
        CALLS 10h,01100h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_TASKING_WORD_LLVM:
        CALLS 09h,0C000h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_LLVM_TASKING_WORD:
        CALLS 10h,02300h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_TASKING_LONG:
        CALLS 09h,0B000h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_LLVM_LONG:
        CALLS 10h,00900h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_LLVM_LONG_TASKING:
        CALLS 10h,01900h
        JMPR cc_UC,PRESSURE_CALL_DONE
PRESSURE_CALL_TASKING_LONG_LLVM:
        CALLS 09h,0D000h
PRESSURE_CALL_DONE:
        PUSH R4
        PUSH R5
        MOV R3,#00h
        POP R11
        POP R10

        POP R1
        MOV R2,DPP3
        CMP R1,R2
        JMPR cc_EQ,PRESSURE_DPP3_OK
        MOV R3,#01h
PRESSURE_DPP3_OK:
        POP R1
        MOV R2,DPP2
        CMP R1,R2
        JMPR cc_EQ,PRESSURE_DPP2_OK
        MOV R3,#01h
PRESSURE_DPP2_OK:
        POP R1
        MOV R2,DPP1
        CMP R1,R2
        JMPR cc_EQ,PRESSURE_DPP1_OK
        MOV R3,#01h
PRESSURE_DPP1_OK:
        POP R1
        MOV R2,DPP0
        CMP R1,R2
        JMPR cc_EQ,PRESSURE_DPP0_OK
        MOV R3,#01h
PRESSURE_DPP0_OK:

        POP R1
        CMP R1,R9
        JMPR cc_EQ,PRESSURE_R9_OK
        MOV R3,#01h
PRESSURE_R9_OK:
        MOV R9,R1
        POP R1
        CMP R1,R8
        JMPR cc_EQ,PRESSURE_R8_OK
        MOV R3,#01h
PRESSURE_R8_OK:
        MOV R8,R1
        POP R1
        CMP R1,R7
        JMPR cc_EQ,PRESSURE_R7_OK
        MOV R3,#01h
PRESSURE_R7_OK:
        MOV R7,R1
        POP R1
        CMP R1,R6
        JMPR cc_EQ,PRESSURE_R6_OK
        MOV R3,#01h
PRESSURE_R6_OK:
        MOV R6,R1

        POP R1
        CMP R1,R0
        JMPR cc_EQ,PRESSURE_R0_OK
        MOV R3,#01h
PRESSURE_R0_OK:
        MOV R0,R1
        POP R1
        MOV R2,SP
        CMP R1,R2
        JMPR cc_EQ,PRESSURE_SP_OK
        MOV R3,#01h
PRESSURE_SP_OK:

        CMP R3,#00h
        JMPR cc_EQ,PRESSURE_ALL_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
        RETS
PRESSURE_ALL_OK:
        MOV R4,R10
        MOV R5,R11
        RETS
PRESSURE_STATE_CALL ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
