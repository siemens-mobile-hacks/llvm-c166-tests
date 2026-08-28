$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    FLOAT32_ARITHMETIC_ABI_BOUNDARY
        EXTERN  _llvm_float32_slot:WORD
        ASSUME  DPP3:SYSTEM

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_float32_eval_proxy
        PUBLIC _llvm_float32_eval_bits_proxy
        PUBLIC _llvm_float32_reverse_proxy
        PUBLIC _llvm_float32_load_external_proxy
        PUBLIC _llvm_float32_store_external_proxy
        PUBLIC _llvm_float32_load_own_proxy
        PUBLIC _llvm_float32_load_own_initial_proxy
        PUBLIC _llvm_float32_store_own_proxy
        PUBLIC _llvm_float32_slot0_proxy
        PUBLIC _llvm_float32_slot1_proxy
        PUBLIC _llvm_float32_slot2_proxy
        PUBLIC _llvm_float32_slot3_proxy
        PUBLIC _llvm_float32_slot4_proxy
        PUBLIC _llvm_float32_call_tasking_proxy
        PUBLIC _llvm_float32_icall_tasking_proxy
        PUBLIC _llvm_float32_cross_tu_proxy
        PUBLIC _tasking_float32_nested_proxy
_llvm_float32_eval_proxy PROC FAR
        CALLS 10h,00100h
        RETS
_llvm_float32_eval_proxy ENDP

; This raw-word entry constructs the Classic ABI frame documented for
; (unsigned int, float, float, unsigned int): R12 remains the leading word,
; both float values and the trailing word occupy the user stack.  TASKING
; stores the IEEE most-significant word first, unlike unsigned long.
_llvm_float32_eval_bits_proxy PROC FAR
        MOV R4,[R0]
        SUB R0,#0Ah
        MOV [R0],R14
        MOV [R0+#02h],R13
        MOV [R0+#04h],R4
        MOV [R0+#06h],R15
        MOV R4,#05A5Ah
        MOV [R0+#08h],R4
        CALLS 10h,00100h
        ADD R0,#0Ah
        ; LLVM returned a Classic float as R4=MSW, R5=LSW.  This proxy is
        ; declared to TASKING as unsigned long, whose register pair is
        ; R4=LSW, R5=MSW.
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float32_eval_bits_proxy ENDP

_llvm_float32_reverse_proxy PROC FAR
        ; The TASKING host exposes raw bits as unsigned long in R12:R13.
        ; Construct an actual Classic float argument on the user stack so the
        ; reverse test cannot accidentally rely on union/integer word order.
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        CALLS 10h,00900h
        ADD R0,#04h
        ; Convert the public float return (MSW:LSW) back to unsigned long
        ; (LSW:MSW) for the TASKING host's raw-bit comparison.
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float32_reverse_proxy ENDP

; Seed TASKING-owned Classic storage with raw MSW:LSW words, then let LLVM
; load it as float and expose the public float return as unsigned long bits.
_llvm_float32_load_external_proxy PROC FAR
        EXTP #PAG _tasking_float_slot,#02h
        MOV _tasking_float_slot,R13
        MOV _tasking_float_slot+02h,R12
        CALLS 10h,01100h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float32_load_external_proxy ENDP

; Pass an actual stack float to LLVM, then observe the physical words written
; into TASKING-owned storage as an unsigned long result.
_llvm_float32_store_external_proxy PROC FAR
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        CALLS 10h,01200h
        ADD R0,#04h
        EXTP #PAG _tasking_float_slot,#02h
        MOV R5,_tasking_float_slot
        MOV R4,_tasking_float_slot+02h
        RETS
_llvm_float32_store_external_proxy ENDP

; Repeat both directions for an LLVM-defined initialized global at 09F00h.
_llvm_float32_load_own_initial_proxy PROC FAR
        CALLS 10h,01300h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float32_load_own_initial_proxy ENDP

_llvm_float32_load_own_proxy PROC FAR
        EXTP #PAG _llvm_float32_slot,#02h
        MOV POF _llvm_float32_slot,R13
        MOV POF (_llvm_float32_slot+02h),R12
        CALLS 10h,01300h
        MOV R1,R4
        MOV R4,R5
        MOV R5,R1
        RETS
_llvm_float32_load_own_proxy ENDP

_llvm_float32_store_own_proxy PROC FAR
        SUB R0,#04h
        MOV [R0],R13
        MOV [R0+#02h],R12
        CALLS 10h,01400h
        ADD R0,#04h
        EXTP #PAG _llvm_float32_slot,#02h
        MOV R5,POF _llvm_float32_slot
        MOV R4,POF (_llvm_float32_slot+02h)
        RETS
_llvm_float32_store_own_proxy ENDP

; Exercise every possible number of register words before the first float.
; Each entry tail-jumps to one state checker, leaving the TASKING-generated
; argument frame and original far return address untouched.
_llvm_float32_slot0_proxy PROC FAR
        MOV R2,#00h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_slot0_proxy ENDP
_llvm_float32_slot1_proxy PROC FAR
        MOV R2,#01h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_slot1_proxy ENDP
_llvm_float32_slot2_proxy PROC FAR
        MOV R2,#02h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_slot2_proxy ENDP
_llvm_float32_slot3_proxy PROC FAR
        MOV R2,#03h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_slot3_proxy ENDP
_llvm_float32_slot4_proxy PROC FAR
        MOV R2,#04h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_slot4_proxy ENDP
_llvm_float32_call_tasking_proxy PROC FAR
        MOV R2,#05h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_call_tasking_proxy ENDP
_llvm_float32_icall_tasking_proxy PROC FAR
        MOV R2,#06h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_icall_tasking_proxy ENDP
_llvm_float32_cross_tu_proxy PROC FAR
        MOV R2,#07h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_llvm_float32_cross_tu_proxy ENDP
_tasking_float32_nested_proxy PROC FAR
        MOV R2,#08h
        JMPS SEG FLOAT32_STATE_CALL,FLOAT32_STATE_CALL
_tasking_float32_nested_proxy ENDP

; Check the complete preserved state around direct, nested and indirect float
; calls.  A mismatch returns C0DE:DEAD as unsigned long; otherwise the public
; float result R4=MSW,R5=LSW is exposed as unsigned long R4=LSW,R5=MSW.
FLOAT32_STATE_CALL PROC FAR
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
        JMPR cc_EQ,FLOAT32_CALL_SLOT0
        CMP R2,#01h
        JMPR cc_EQ,FLOAT32_CALL_SLOT1
        CMP R2,#02h
        JMPR cc_EQ,FLOAT32_CALL_SLOT2
        CMP R2,#03h
        JMPR cc_EQ,FLOAT32_CALL_SLOT3
        CMP R2,#04h
        JMPR cc_EQ,FLOAT32_CALL_SLOT4
        CMP R2,#05h
        JMPR cc_EQ,FLOAT32_CALL_TASKING
        CMP R2,#06h
        JMPR cc_EQ,FLOAT32_ICALL_TASKING
        CMP R2,#07h
        JMPR cc_EQ,FLOAT32_CALL_CROSS_TU
        CALLS 0Bh,06000h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_CROSS_TU:
        CALLS 10h,01C00h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_SLOT0:
        CALLS 10h,01500h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_SLOT1:
        CALLS 10h,01600h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_SLOT2:
        CALLS 10h,01700h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_SLOT3:
        CALLS 10h,01800h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_SLOT4:
        CALLS 10h,01900h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_CALL_TASKING:
        CALLS 10h,01A00h
        JMPR cc_UC,FLOAT32_CALL_DONE
FLOAT32_ICALL_TASKING:
        CALLS 10h,01B00h
FLOAT32_CALL_DONE:
        PUSH R4
        PUSH R5
        MOV R3,#00h
        POP R11
        POP R10

        POP R1
        MOV R2,DPP3
        CMP R1,R2
        JMPR cc_EQ,FLOAT32_DPP3_OK
        MOV R3,#01h
FLOAT32_DPP3_OK:
        POP R1
        MOV R2,DPP2
        CMP R1,R2
        JMPR cc_EQ,FLOAT32_DPP2_OK
        MOV R3,#01h
FLOAT32_DPP2_OK:
        POP R1
        MOV R2,DPP1
        CMP R1,R2
        JMPR cc_EQ,FLOAT32_DPP1_OK
        MOV R3,#01h
FLOAT32_DPP1_OK:
        POP R1
        MOV R2,DPP0
        CMP R1,R2
        JMPR cc_EQ,FLOAT32_DPP0_OK
        MOV R3,#01h
FLOAT32_DPP0_OK:
        POP R1
        CMP R1,R9
        JMPR cc_EQ,FLOAT32_R9_OK
        MOV R3,#01h
FLOAT32_R9_OK:
        MOV R9,R1
        POP R1
        CMP R1,R8
        JMPR cc_EQ,FLOAT32_R8_OK
        MOV R3,#01h
FLOAT32_R8_OK:
        MOV R8,R1
        POP R1
        CMP R1,R7
        JMPR cc_EQ,FLOAT32_R7_OK
        MOV R3,#01h
FLOAT32_R7_OK:
        MOV R7,R1
        POP R1
        CMP R1,R6
        JMPR cc_EQ,FLOAT32_R6_OK
        MOV R3,#01h
FLOAT32_R6_OK:
        MOV R6,R1
        POP R1
        CMP R1,R0
        JMPR cc_EQ,FLOAT32_R0_OK
        MOV R3,#01h
FLOAT32_R0_OK:
        MOV R0,R1
        POP R1
        MOV R2,SP
        CMP R1,R2
        JMPR cc_EQ,FLOAT32_SP_OK
        MOV R3,#01h
FLOAT32_SP_OK:
        CMP R3,#00h
        JMPR cc_EQ,FLOAT32_STATE_OK
        MOV R4,#0DEADh
        MOV R5,#0C0DEh
        RETS
FLOAT32_STATE_OK:
        MOV R4,R11
        MOV R5,R10
        RETS
FLOAT32_STATE_CALL ENDP
LLVM_PROXY_PR ENDS

TASKING_FLOAT_STORAGE SECTION DATA WORD PUBLIC 'FLOATSTORE'
_tasking_float_slot LABEL WORD
        DS 04h
        PUBLIC _tasking_float_slot
TASKING_FLOAT_STORAGE ENDS

; Exact Classic float objects for the TASKING C caller.  Keeping these words
; directly in the downloaded image avoids relying on a hosted C initializer
; startup that this CRT-free test intentionally does not run.
TASKING_FLOAT_CALL_VALUES SECTION DATA WORD PUBLIC 'FLOATCALLROM'
_tasking_float_call_values LABEL WORD
        DW 0000h,0000h
        DW 08000h,0000h
        DW 03F80h,0000h
        DW 0C020h,0000h
        DW 03F00h,0000h
        DW 0477Fh,0E000h
        DW 04049h,00FDBh
        DW 00080h,0000h
        PUBLIC _tasking_float_call_values
TASKING_FLOAT_CALL_VALUES ENDS

        REGDEF R0-R15
        END
