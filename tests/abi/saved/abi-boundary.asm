$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    SAVED_ABI_BOUNDARY

SAVED_ABI_BOUNDARY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC  _llvm_entry_saved_proxy

_llvm_entry_saved_proxy PROC FAR
        MOV     R6,#06006h
        MOV     R7,#07007h
        MOV     R8,#08008h
        MOV     R9,#09009h
        CALLS   10h,0000h

        MOV     R1,#06006h
        CMP     R6,R1
        JMPR    cc_NE,SAVED_FAILED
        MOV     R1,#07007h
        CMP     R7,R1
        JMPR    cc_NE,SAVED_FAILED
        MOV     R1,#08008h
        CMP     R8,R1
        JMPR    cc_NE,SAVED_FAILED
        MOV     R1,#09009h
        CMP     R9,R1
        JMPR    cc_NE,SAVED_FAILED
        RETS

SAVED_FAILED:
        MOV     R4,#0DEADh
        RETS
_llvm_entry_saved_proxy ENDP

SAVED_ABI_BOUNDARY_PR ENDS

        REGDEF  R0-R15
        END
