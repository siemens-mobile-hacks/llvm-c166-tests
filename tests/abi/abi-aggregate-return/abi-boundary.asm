$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME AGGREGATE_STATE
        ASSUME DPP3:SYSTEM

@IF( @TASKING_MODEL_IS_MEDIUM )
        EXTERN _hash_tasking_returns:NEAR
        EXTERN _hash_llvm_returns:NEAR
        EXTERN _llvm_medium_entry_proxy:NEAR
@ELSE
        EXTERN _hash_tasking_returns:FAR
        EXTERN _hash_llvm_returns:FAR
        EXTERN _llvm_entry_proxy:FAR
@ENDI

AGGREGATE_STATE_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _aggregate_state_call
@IF( @TASKING_MODEL_IS_MEDIUM )
_aggregate_state_call PROC NEAR
@ELSE
_aggregate_state_call PROC FAR
@ENDI
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
        MOV R6,#06666h
        MOV R7,#07777h
        MOV R8,#08888h
        MOV R9,#09999h

        CMP R13,#0
        JMPR cc_EQ,STATE_REFERENCE
        CMP R13,#1
        JMPR cc_EQ,STATE_TASKING_LLVM
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,_llvm_medium_entry_proxy
@ELSE
        CALLS SEG _llvm_entry_proxy,_llvm_entry_proxy
@ENDI
        JMPR cc_UC,STATE_CHECK
STATE_REFERENCE:
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,_hash_tasking_returns
@ELSE
        CALLS SEG _hash_tasking_returns,_hash_tasking_returns
@ENDI
        JMPR cc_UC,STATE_CHECK
STATE_TASKING_LLVM:
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,_hash_llvm_returns
@ELSE
        CALLS SEG _hash_llvm_returns,_hash_llvm_returns
@ENDI
STATE_CHECK:
        ; R4 is the C hash; a nonzero high word makes the host check fail.
        MOV R5,#0
        POP R1
        MOV R2,DPP3
        CMP R1,R2
        JMPR cc_EQ,STATE_DPP3_OK
        OR R5,#01h
STATE_DPP3_OK:
        MOV DPP3,R1
        POP R1
        MOV R2,DPP2
        CMP R1,R2
        JMPR cc_EQ,STATE_DPP2_OK
        OR R5,#02h
STATE_DPP2_OK:
        MOV DPP2,R1
        POP R1
        MOV R2,DPP1
        CMP R1,R2
        JMPR cc_EQ,STATE_DPP1_OK
        OR R5,#04h
STATE_DPP1_OK:
        MOV DPP1,R1
        POP R1
        MOV R2,DPP0
        CMP R1,R2
        JMPR cc_EQ,STATE_DPP0_OK
        OR R5,#08h
STATE_DPP0_OK:
        MOV DPP0,R1
        CMP R9,#09999h
        JMPR cc_EQ,STATE_R9_OK
        OR R5,#010h
STATE_R9_OK:
        POP R9
        CMP R8,#08888h
        JMPR cc_EQ,STATE_R8_OK
        OR R5,#020h
STATE_R8_OK:
        POP R8
        CMP R7,#07777h
        JMPR cc_EQ,STATE_R7_OK
        OR R5,#040h
STATE_R7_OK:
        POP R7
        CMP R6,#06666h
        JMPR cc_EQ,STATE_R6_OK
        OR R5,#080h
STATE_R6_OK:
        POP R6
        POP R1
        CMP R0,R1
        JMPR cc_EQ,STATE_R0_OK
        OR R5,#0100h
STATE_R0_OK:
        MOV R0,R1
        POP R1
        MOV R2,SP
        CMP R1,R2
        JMPR cc_EQ,STATE_SP_OK
        OR R5,#0200h
STATE_SP_OK:
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_aggregate_state_call ENDP
AGGREGATE_STATE_PR ENDS
        END
