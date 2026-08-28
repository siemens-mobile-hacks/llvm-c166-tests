$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_FAR_MEMORY_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_far_apply_proxy
        PUBLIC _llvm_far_advance_proxy
        PUBLIC _llvm_far_reverse_apply_proxy
        PUBLIC _llvm_far_reverse_advance_proxy
        PUBLIC _llvm_far_advance_bits_proxy
        PUBLIC _tasking_far_advance_bits_proxy
        PUBLIC _llvm_far_reverse_advance_bits_proxy
        PUBLIC _llvm_far_retreat_bits_proxy
        PUBLIC _tasking_far_retreat_bits_proxy
        PUBLIC _llvm_far_reverse_retreat_bits_proxy
        PUBLIC _llvm_far_equal_raw_proxy
        PUBLIC _tasking_far_equal_raw_proxy
        PUBLIC _llvm_far_not_equal_raw_proxy
        PUBLIC _tasking_far_not_equal_raw_proxy
        PUBLIC _llvm_far_less_raw_proxy
        PUBLIC _tasking_far_less_raw_proxy
        PUBLIC _llvm_far_less_equal_raw_proxy
        PUBLIC _tasking_far_less_equal_raw_proxy
        PUBLIC _llvm_far_greater_raw_proxy
        PUBLIC _tasking_far_greater_raw_proxy
        PUBLIC _llvm_far_greater_equal_raw_proxy
        PUBLIC _tasking_far_greater_equal_raw_proxy
        PUBLIC _llvm_far_difference_raw_proxy
        PUBLIC _tasking_far_difference_raw_proxy
        PUBLIC _llvm_far_is_null_raw_proxy
        PUBLIC _tasking_far_is_null_raw_proxy

_llvm_far_apply_proxy PROC FAR
        CALLS 10h,00100h
        RETS
_llvm_far_apply_proxy ENDP

_llvm_far_advance_proxy PROC FAR
        CALLS 10h,00500h
        RETS
_llvm_far_advance_proxy ENDP

_llvm_far_reverse_apply_proxy PROC FAR
        CALLS 10h,00600h
        RETS
_llvm_far_reverse_apply_proxy ENDP

_llvm_far_reverse_advance_proxy PROC FAR
        CALLS 10h,00700h
        RETS
_llvm_far_reverse_advance_proxy ENDP

; Classic far pointers and unsigned long return the same low/high words in
; R4:R5.  These aliases expose the raw pointer result without C type punning.
_llvm_far_advance_bits_proxy PROC FAR
        CALLS 10h,00500h
        RETS
_llvm_far_advance_bits_proxy ENDP

_tasking_far_advance_bits_proxy PROC FAR
        CALLS 09h,09000h
        RETS
_tasking_far_advance_bits_proxy ENDP

_llvm_far_reverse_advance_bits_proxy PROC FAR
        CALLS 10h,00700h
        RETS
_llvm_far_reverse_advance_bits_proxy ENDP

_llvm_far_retreat_bits_proxy PROC FAR
        CALLS 10h,00800h
        RETS
_llvm_far_retreat_bits_proxy ENDP

_tasking_far_retreat_bits_proxy PROC FAR
        CALLS 09h,0A000h
        RETS
_tasking_far_retreat_bits_proxy ENDP

_llvm_far_reverse_retreat_bits_proxy PROC FAR
        CALLS 10h,00880h
        RETS
_llvm_far_reverse_retreat_bits_proxy ENDP

; These raw wrappers deliberately preserve R12:R15.  Four word arguments have
; exactly the same register layout as two far pointers, while two words have
; exactly the same layout as one far pointer.  No ABI value is marshalled.
_llvm_far_equal_raw_proxy PROC FAR
        CALLS 10h,00440h
        RETS
_llvm_far_equal_raw_proxy ENDP

_tasking_far_equal_raw_proxy PROC FAR
        CALLS 09h,0B000h
        RETS
_tasking_far_equal_raw_proxy ENDP

_llvm_far_not_equal_raw_proxy PROC FAR
        CALLS 10h,03100h
        RETS
_llvm_far_not_equal_raw_proxy ENDP

_tasking_far_not_equal_raw_proxy PROC FAR
        CALLS 09h,0E000h
        RETS
_tasking_far_not_equal_raw_proxy ENDP

_llvm_far_less_raw_proxy PROC FAR
        CALLS 10h,00522h
        RETS
_llvm_far_less_raw_proxy ENDP

_tasking_far_less_raw_proxy PROC FAR
        CALLS 09h,0C000h
        RETS
_tasking_far_less_raw_proxy ENDP

_llvm_far_less_equal_raw_proxy PROC FAR
        CALLS 10h,03200h
        RETS
_llvm_far_less_equal_raw_proxy ENDP

_tasking_far_less_equal_raw_proxy PROC FAR
        CALLS 09h,0F000h
        RETS
_tasking_far_less_equal_raw_proxy ENDP

_llvm_far_greater_raw_proxy PROC FAR
        CALLS 10h,03300h
        RETS
_llvm_far_greater_raw_proxy ENDP

_tasking_far_greater_raw_proxy PROC FAR
        CALLS 0Ah,00000h
        RETS
_tasking_far_greater_raw_proxy ENDP

_llvm_far_greater_equal_raw_proxy PROC FAR
        CALLS 10h,03400h
        RETS
_llvm_far_greater_equal_raw_proxy ENDP

_tasking_far_greater_equal_raw_proxy PROC FAR
        CALLS 0Ah,01000h
        RETS
_tasking_far_greater_equal_raw_proxy ENDP

_llvm_far_difference_raw_proxy PROC FAR
        CALLS 10h,03500h
        RETS
_llvm_far_difference_raw_proxy ENDP

_tasking_far_difference_raw_proxy PROC FAR
        CALLS 0Ah,02000h
        RETS
_tasking_far_difference_raw_proxy ENDP

_llvm_far_is_null_raw_proxy PROC FAR
        CALLS 10h,00624h
        RETS
_llvm_far_is_null_raw_proxy ENDP

_tasking_far_is_null_raw_proxy PROC FAR
        CALLS 09h,0D000h
        RETS
_tasking_far_is_null_raw_proxy ENDP
LLVM_PROXY_PR ENDS

FAR_PAGE1_RESERVATION SECTION DATA WORD PUBLIC 'FARPG1'
_far_page1_arena LABEL WORD
        DS 030h
        PUBLIC _far_page1_arena
FAR_PAGE1_RESERVATION ENDS

FAR_PAGE4_RESERVATION SECTION DATA WORD PUBLIC 'FARPG4'
_far_page4_arena LABEL WORD
        DS 030h
        PUBLIC _far_page4_arena
FAR_PAGE4_RESERVATION ENDS

        REGDEF R0-R15
        END
