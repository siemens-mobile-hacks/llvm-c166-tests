$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$CASE
$NOEXPANDREGBANK

        NAME    ABI_TUPLE_RELOAD_BOUNDARY
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_tuple_reload_low_state_proxy
        PUBLIC _llvm_tuple_reload_high_state_proxy

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_tuple_reload_low_state_proxy PROC NEAR
@ELSE
_llvm_tuple_reload_low_state_proxy PROC FAR
@ENDI
        MOV R2,#00h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,TUPLE_RELOAD_STATE_CALL
@ELSE
        CALLS SEG TUPLE_RELOAD_STATE_CALL,TUPLE_RELOAD_STATE_CALL
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_tuple_reload_low_state_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
_llvm_tuple_reload_high_state_proxy PROC NEAR
@ELSE
_llvm_tuple_reload_high_state_proxy PROC FAR
@ENDI
        MOV R2,#01h
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,TUPLE_RELOAD_STATE_CALL
@ELSE
        CALLS SEG TUPLE_RELOAD_STATE_CALL,TUPLE_RELOAD_STATE_CALL
@ENDI
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
_llvm_tuple_reload_high_state_proxy ENDP

@IF( @TASKING_MODEL_IS_MEDIUM )
TUPLE_RELOAD_STATE_CALL PROC NEAR
@ELSE
TUPLE_RELOAD_STATE_CALL PROC FAR
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

        CMP R2,#00h
        JMPR cc_EQ,TUPLE_RELOAD_CALL_LOW
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0D200h
@ELSE
        CALLS 10h,1100h
@ENDI
        JMPR cc_UC,TUPLE_RELOAD_CALL_DONE
TUPLE_RELOAD_CALL_LOW:
@IF( @TASKING_MODEL_IS_MEDIUM )
        CALLA cc_UC,0C200h
@ELSE
        CALLS 10h,0100h
@ENDI
TUPLE_RELOAD_CALL_DONE:
        PUSH R4
        MOV R3,#00h
        POP R10

        POP R1
        MOV R2,DPP3
        CMP R1,R2
        JMPR cc_EQ,TUPLE_RELOAD_DPP3_OK
        MOV R3,#01h
TUPLE_RELOAD_DPP3_OK:
        POP R1
        MOV R2,DPP2
        CMP R1,R2
        JMPR cc_EQ,TUPLE_RELOAD_DPP2_OK
        MOV R3,#01h
TUPLE_RELOAD_DPP2_OK:
        POP R1
        MOV R2,DPP1
        CMP R1,R2
        JMPR cc_EQ,TUPLE_RELOAD_DPP1_OK
        MOV R3,#01h
TUPLE_RELOAD_DPP1_OK:
        POP R1
        MOV R2,DPP0
        CMP R1,R2
        JMPR cc_EQ,TUPLE_RELOAD_DPP0_OK
        MOV R3,#01h
TUPLE_RELOAD_DPP0_OK:

        POP R1
        CMP R1,R9
        JMPR cc_EQ,TUPLE_RELOAD_R9_OK
        MOV R3,#01h
TUPLE_RELOAD_R9_OK:
        MOV R9,R1
        POP R1
        CMP R1,R8
        JMPR cc_EQ,TUPLE_RELOAD_R8_OK
        MOV R3,#01h
TUPLE_RELOAD_R8_OK:
        MOV R8,R1
        POP R1
        CMP R1,R7
        JMPR cc_EQ,TUPLE_RELOAD_R7_OK
        MOV R3,#01h
TUPLE_RELOAD_R7_OK:
        MOV R7,R1
        POP R1
        CMP R1,R6
        JMPR cc_EQ,TUPLE_RELOAD_R6_OK
        MOV R3,#01h
TUPLE_RELOAD_R6_OK:
        MOV R6,R1

        POP R1
        CMP R1,R0
        JMPR cc_EQ,TUPLE_RELOAD_R0_OK
        MOV R3,#01h
TUPLE_RELOAD_R0_OK:
        MOV R0,R1
        POP R1
        MOV R2,SP
        CMP R1,R2
        JMPR cc_EQ,TUPLE_RELOAD_SP_OK
        MOV R3,#01h
TUPLE_RELOAD_SP_OK:

        CMP R3,#00h
        JMPR cc_EQ,TUPLE_RELOAD_ALL_OK
        MOV R4,#0C0DEh
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
TUPLE_RELOAD_ALL_OK:
        MOV R4,R10
@IF( @TASKING_MODEL_IS_MEDIUM )
        RET
@ELSE
        RETS
@ENDI
TUPLE_RELOAD_STATE_CALL ENDP
LLVM_PROXY_PR ENDS

        REGDEF R0-R15
        END
