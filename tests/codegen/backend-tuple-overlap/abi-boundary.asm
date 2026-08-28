$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    BACKEND_TUPLE_OVERLAP_ABI_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _copy_pair_up_state_proxy
        PUBLIC _zext_pair_high_overlap_state_proxy
        PUBLIC _sext_pair_high_overlap_state_proxy
        PUBLIC _copy_pair_down_state_proxy

_copy_pair_up_state_proxy PROC FAR
        MOV R2,#00h
        CALLS SEG TUPLE_STATE_CALL,TUPLE_STATE_CALL
        RETS
_copy_pair_up_state_proxy ENDP
_zext_pair_high_overlap_state_proxy PROC FAR
        MOV R2,#01h
        CALLS SEG TUPLE_STATE_CALL,TUPLE_STATE_CALL
        RETS
_zext_pair_high_overlap_state_proxy ENDP
_sext_pair_high_overlap_state_proxy PROC FAR
        MOV R2,#02h
        CALLS SEG TUPLE_STATE_CALL,TUPLE_STATE_CALL
        RETS
_sext_pair_high_overlap_state_proxy ENDP
_copy_pair_down_state_proxy PROC FAR
        MOV R2,#03h
        CALLS SEG TUPLE_STATE_CALL,TUPLE_STATE_CALL
        RETS
_copy_pair_down_state_proxy ENDP

TUPLE_STATE_CALL PROC FAR
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
        JMPR cc_EQ,TUPLE_COPY_UP
        CMP R2,#01h
        JMPR cc_EQ,TUPLE_ZEXT
        CMP R2,#02h
        JMPR cc_EQ,TUPLE_SEXT
        MOV R2,R12
        MOV R3,R13
        CALLS 18h,0014h
        MOV R10,R1
        MOV R11,R2
        JMPR cc_UC,TUPLE_CALL_DONE
TUPLE_COPY_UP:
        MOV R1,R12
        MOV R2,R13
        CALLS 18h,0000h
        MOV R10,R2
        MOV R11,R3
        JMPR cc_UC,TUPLE_CALL_DONE
TUPLE_ZEXT:
        MOV R2,R12
        CALLS 18h,0006h
        MOV R10,R1
        MOV R11,R2
        JMPR cc_UC,TUPLE_CALL_DONE
TUPLE_SEXT:
        MOV R2,R12
        CALLS 18h,000Ch
        MOV R10,R1
        MOV R11,R2
TUPLE_CALL_DONE:
        MOV R3,#00h

        POP R1
        MOV R2,DPP3
        CMP R1,R2
        JMPR cc_EQ,TUPLE_DPP3_OK
        MOV R3,#01h
TUPLE_DPP3_OK:
        POP R1
        MOV R2,DPP2
        CMP R1,R2
        JMPR cc_EQ,TUPLE_DPP2_OK
        MOV R3,#01h
TUPLE_DPP2_OK:
        POP R1
        MOV R2,DPP1
        CMP R1,R2
        JMPR cc_EQ,TUPLE_DPP1_OK
        MOV R3,#01h
TUPLE_DPP1_OK:
        POP R1
        MOV R2,DPP0
        CMP R1,R2
        JMPR cc_EQ,TUPLE_DPP0_OK
        MOV R3,#01h
TUPLE_DPP0_OK:
        POP R1
        CMP R1,R9
        JMPR cc_EQ,TUPLE_R9_OK
        MOV R3,#01h
TUPLE_R9_OK:
        MOV R9,R1
        POP R1
        CMP R1,R8
        JMPR cc_EQ,TUPLE_R8_OK
        MOV R3,#01h
TUPLE_R8_OK:
        MOV R8,R1
        POP R1
        CMP R1,R7
        JMPR cc_EQ,TUPLE_R7_OK
        MOV R3,#01h
TUPLE_R7_OK:
        MOV R7,R1
        POP R1
        CMP R1,R6
        JMPR cc_EQ,TUPLE_R6_OK
        MOV R3,#01h
TUPLE_R6_OK:
        MOV R6,R1
        POP R1
        CMP R1,R0
        JMPR cc_EQ,TUPLE_R0_OK
        MOV R3,#01h
TUPLE_R0_OK:
        MOV R0,R1
        POP R1
        MOV R2,SP
        CMP R1,R2
        JMPR cc_EQ,TUPLE_SP_OK
        MOV R3,#01h
TUPLE_SP_OK:
        CMP R3,#00h
        JMPR cc_EQ,TUPLE_ALL_OK
        MOV R4,#0C0DEh
        MOV R5,#0DEADh
        RETS
TUPLE_ALL_OK:
        MOV R4,R10
        MOV R5,R11
        RETS
TUPLE_STATE_CALL ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
