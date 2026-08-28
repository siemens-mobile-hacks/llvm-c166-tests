$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_MIXED_RECURSION_BOUNDARY
        ASSUME  DPP3:SYSTEM

MIXED_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_mixed_state_proxy
        PUBLIC _tasking_mixed_state_proxy
        PUBLIC _llvm_mixed_bridge

_llvm_mixed_bridge PROC FAR
        CALLS   10h,03000h
        RETS
_llvm_mixed_bridge ENDP

_llvm_mixed_state_proxy PROC FAR
        MOV     R2,#00h
        CALLS   SEG MIXED_STATE_CALL,MIXED_STATE_CALL
        RETS
_llvm_mixed_state_proxy ENDP

_tasking_mixed_state_proxy PROC FAR
        MOV     R2,#01h
        CALLS   SEG MIXED_STATE_CALL,MIXED_STATE_CALL
        RETS
_tasking_mixed_state_proxy ENDP

MIXED_STATE_CALL PROC FAR
        MOV     R1,SP
        PUSH    R1
        PUSH    R0
        PUSH    R6
        PUSH    R7
        PUSH    R8
        PUSH    R9
        MOV     R1,DPP0
        PUSH    R1
        MOV     R1,DPP1
        PUSH    R1
        MOV     R1,DPP2
        PUSH    R1
        MOV     R1,DPP3
        PUSH    R1

        CMP     R2,#00h
        JMPR    cc_NE,MIXED_CALL_TASKING
        CALLS   10h,03000h
        JMPR    cc_UC,MIXED_CALL_DONE
MIXED_CALL_TASKING:
        CALLS   0Bh,07000h
MIXED_CALL_DONE:
        PUSH    R4
        PUSH    R5
        MOV     R3,#00h
        POP     R11
        POP     R10

        POP     R1
        MOV     R2,DPP3
        CMP     R1,R2
        JMPR    cc_EQ,MIXED_DPP3_OK
        MOV     R3,#01h
MIXED_DPP3_OK:
        POP     R1
        MOV     R2,DPP2
        CMP     R1,R2
        JMPR    cc_EQ,MIXED_DPP2_OK
        MOV     R3,#01h
MIXED_DPP2_OK:
        POP     R1
        MOV     R2,DPP1
        CMP     R1,R2
        JMPR    cc_EQ,MIXED_DPP1_OK
        MOV     R3,#01h
MIXED_DPP1_OK:
        POP     R1
        MOV     R2,DPP0
        CMP     R1,R2
        JMPR    cc_EQ,MIXED_DPP0_OK
        MOV     R3,#01h
MIXED_DPP0_OK:
        POP     R1
        CMP     R1,R9
        JMPR    cc_EQ,MIXED_R9_OK
        MOV     R3,#01h
MIXED_R9_OK:
        MOV     R9,R1
        POP     R1
        CMP     R1,R8
        JMPR    cc_EQ,MIXED_R8_OK
        MOV     R3,#01h
MIXED_R8_OK:
        MOV     R8,R1
        POP     R1
        CMP     R1,R7
        JMPR    cc_EQ,MIXED_R7_OK
        MOV     R3,#01h
MIXED_R7_OK:
        MOV     R7,R1
        POP     R1
        CMP     R1,R6
        JMPR    cc_EQ,MIXED_R6_OK
        MOV     R3,#01h
MIXED_R6_OK:
        MOV     R6,R1
        POP     R1
        CMP     R1,R0
        JMPR    cc_EQ,MIXED_R0_OK
        MOV     R3,#01h
MIXED_R0_OK:
        MOV     R0,R1
        POP     R1
        MOV     R2,SP
        CMP     R1,R2
        JMPR    cc_EQ,MIXED_SP_OK
        MOV     R3,#01h
MIXED_SP_OK:
        CMP     R3,#00h
        JMPR    cc_EQ,MIXED_STATE_OK
        MOV     R4,#0C0DEh
        MOV     R5,#0DEADh
        RETS
MIXED_STATE_OK:
        MOV     R4,R10
        MOV     R5,R11
        RETS
MIXED_STATE_CALL ENDP

MIXED_PROXY_PR ENDS

TASKING_MIXED_FLOAT_VALUES SECTION DATA WORD PUBLIC 'MIXFLOAT'
_tasking_mixed_float_values LABEL WORD
        PUBLIC  _tasking_mixed_float_values
        DW      03F80h,00000h
        DW      0C020h,00000h
        DW      08000h,00000h
        DW      07FC1h,02345h
TASKING_MIXED_FLOAT_VALUES ENDS

        REGDEF  R0-R15
        END
