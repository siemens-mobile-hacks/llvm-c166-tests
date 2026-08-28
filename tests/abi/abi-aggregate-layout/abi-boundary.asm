$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)

        NAME    ABI_AGGREGATE_LAYOUT_BOUNDARY

LLVM_PROXY_PR  SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC  _llvm_layout_chars0_proxy
        PUBLIC  _llvm_layout_padded1_proxy
        PUBLIC  _llvm_layout_nested2_proxy
        PUBLIC  _llvm_layout_union3_proxy
        PUBLIC  _llvm_layout_packed0_proxy
        PUBLIC  _llvm_layout_bits3_proxy
        PUBLIC  _llvm_layout_packed_bits0_proxy
        PUBLIC  _llvm_layout_bit_edges2_proxy
        PUBLIC  _llvm_layout_nested_packed1_proxy
        PUBLIC  _tasking_layout_packed_bits0_abi_proxy
        PUBLIC  _llvm_layout_packed_bits0_abi_proxy
        PUBLIC  _tasking_layout_bit_edges2_abi_proxy
        PUBLIC  _llvm_layout_bit_edges2_abi_proxy

_llvm_layout_chars0_proxy PROC FAR
        CALLS   10h,0100h
        RETS
_llvm_layout_chars0_proxy ENDP

_llvm_layout_padded1_proxy PROC FAR
        CALLS   10h,0300h
        RETS
_llvm_layout_padded1_proxy ENDP

_llvm_layout_nested2_proxy PROC FAR
        CALLS   10h,0500h
        RETS
_llvm_layout_nested2_proxy ENDP

_llvm_layout_union3_proxy PROC FAR
        CALLS   10h,0700h
        RETS
_llvm_layout_union3_proxy ENDP

_llvm_layout_packed0_proxy PROC FAR
        CALLS   10h,0900h
        RETS
_llvm_layout_packed0_proxy ENDP

_llvm_layout_bits3_proxy PROC FAR
        CALLS   10h,0B00h
        RETS
_llvm_layout_bits3_proxy ENDP

_llvm_layout_packed_bits0_proxy PROC FAR
        CALLS   10h,0D00h
        RETS
_llvm_layout_packed_bits0_proxy ENDP

_llvm_layout_bit_edges2_proxy PROC FAR
        CALLS   10h,1800h
        RETS
_llvm_layout_bit_edges2_proxy ENDP

_llvm_layout_nested_packed1_proxy PROC FAR
        CALLS   10h,2800h
        RETS
_llvm_layout_nested_packed1_proxy ENDP

; Build 753's C caller copies a three-byte _packed parameter without the
; fourth byte required by the callee's word-sized stack slot.  These two
; seed-independent callers accept four ordinary far pointers in R12:R13,
; R14:R15, [R0]:[R0+2] and [R0+4]:[R0+6], plus the tail at [R0+8].  They copy
; exactly three object bytes into four-byte ABI slots and make the same call
; once to the TASKING callee and once to the LLVM callee. R6/R7 are Classic
; callee-saved registers, so preserve them around the byte-copy scratch use.
_tasking_layout_packed_bits0_abi_proxy PROC FAR
        MOV     R10,R6
        MOV     R11,R7
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R3,[R0+#04H]
        MOV     R4,[R0+#06H]
        MOV     R5,[R0+#08H]
        MOV     [-R0],R10
        MOV     [-R0],R11
        SUB     R0,#0FH
        SUB     R0,#03H

        EXTP    R13,#03H
        MOVB    RL6,[R12]
        MOVB    RH6,[R12+#01H]
        MOVB    RL7,[R12+#02H]
        MOVB    [R0],RL6
        MOVB    [R0+#01H],RH6
        MOVB    [R0+#02H],RL7

        EXTP    R15,#03H
        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        EXTP    R2,#03H
        MOVB    RL6,[R1]
        MOVB    RH6,[R1+#01H]
        MOVB    RL7,[R1+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        EXTP    R4,#03H
        MOVB    RL6,[R3]
        MOVB    RH6,[R3+#01H]
        MOVB    RL7,[R3+#02H]
        MOVB    [R0+#0CH],RL6
        MOVB    [R0+#0DH],RH6
        MOVB    [R0+#0EH],RL7

        MOV     R6,#00H
        MOVB    [R0+#03H],RL6
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOVB    [R0+#0FH],RL6
        MOV     [R0+#10H],R5
        CALLS   SEG _tasking_layout_packed_bits0,_tasking_layout_packed_bits0
        ADD     R0,#0FH
        ADD     R0,#03H
        MOV     R7,[R0+]
        MOV     R6,[R0+]
        RETS
_tasking_layout_packed_bits0_abi_proxy ENDP

_llvm_layout_packed_bits0_abi_proxy PROC FAR
        MOV     R10,R6
        MOV     R11,R7
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R3,[R0+#04H]
        MOV     R4,[R0+#06H]
        MOV     R5,[R0+#08H]
        MOV     [-R0],R10
        MOV     [-R0],R11
        SUB     R0,#0FH
        SUB     R0,#03H

        EXTP    R13,#03H
        MOVB    RL6,[R12]
        MOVB    RH6,[R12+#01H]
        MOVB    RL7,[R12+#02H]
        MOVB    [R0],RL6
        MOVB    [R0+#01H],RH6
        MOVB    [R0+#02H],RL7

        EXTP    R15,#03H
        MOVB    RL6,[R14]
        MOVB    RH6,[R14+#01H]
        MOVB    RL7,[R14+#02H]
        MOVB    [R0+#04H],RL6
        MOVB    [R0+#05H],RH6
        MOVB    [R0+#06H],RL7

        EXTP    R2,#03H
        MOVB    RL6,[R1]
        MOVB    RH6,[R1+#01H]
        MOVB    RL7,[R1+#02H]
        MOVB    [R0+#08H],RL6
        MOVB    [R0+#09H],RH6
        MOVB    [R0+#0AH],RL7

        EXTP    R4,#03H
        MOVB    RL6,[R3]
        MOVB    RH6,[R3+#01H]
        MOVB    RL7,[R3+#02H]
        MOVB    [R0+#0CH],RL6
        MOVB    [R0+#0DH],RH6
        MOVB    [R0+#0EH],RL7

        MOV     R6,#00H
        MOVB    [R0+#03H],RL6
        MOVB    [R0+#07H],RL6
        MOVB    [R0+#0BH],RL6
        MOVB    [R0+#0FH],RL6
        MOV     [R0+#10H],R5
        CALLS   10h,0D00h
        ADD     R0,#0FH
        ADD     R0,#03H
        MOV     R7,[R0+]
        MOV     R6,[R0+]
        RETS
_llvm_layout_packed_bits0_abi_proxy ENDP

; Build 753 copies trailing_bits from two bytes after its actual local base.
; These callers receive head0/head1 in R12/R13, the first ordinary far pointer
; in R14:R15, then three far pointers and tail on the user stack. They
; materialize the exact 4/2/4/4/2 byte stack stream once for each callee and
; preserve the Classic callee-saved R6/R7 pair.
_tasking_layout_bit_edges2_abi_proxy PROC FAR
        MOV     R10,R6
        MOV     R11,R7
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R3,[R0+#04H]
        MOV     R4,[R0+#06H]
        MOV     R5,[R0+#08H]
        MOV     R6,[R0+#0AH]
        MOV     R7,[R0+#0CH]
        MOV     [-R0],R10
        MOV     [-R0],R11
        SUB     R0,#10H

        EXTP    R15,#02H
        MOV     R10,[R14]
        MOV     R11,[R14+#02H]
        MOV     [R0],R10
        MOV     [R0+#02H],R11

        EXTP    R2,#01H
        MOV     R10,[R1]
        MOV     [R0+#04H],R10

        EXTP    R4,#02H
        MOV     R10,[R3]
        MOV     R11,[R3+#02H]
        MOV     [R0+#06H],R10
        MOV     [R0+#08H],R11

        EXTP    R6,#02H
        MOV     R10,[R5]
        MOV     R11,[R5+#02H]
        MOV     [R0+#0AH],R10
        MOV     [R0+#0CH],R11
        MOV     [R0+#0EH],R7
        CALLS   08h,7000h
        ADD     R0,#10H
        MOV     R7,[R0+]
        MOV     R6,[R0+]
        RETS
_tasking_layout_bit_edges2_abi_proxy ENDP

_llvm_layout_bit_edges2_abi_proxy PROC FAR
        MOV     R10,R6
        MOV     R11,R7
        MOV     R1,[R0]
        MOV     R2,[R0+#02H]
        MOV     R3,[R0+#04H]
        MOV     R4,[R0+#06H]
        MOV     R5,[R0+#08H]
        MOV     R6,[R0+#0AH]
        MOV     R7,[R0+#0CH]
        MOV     [-R0],R10
        MOV     [-R0],R11
        SUB     R0,#10H

        EXTP    R15,#02H
        MOV     R10,[R14]
        MOV     R11,[R14+#02H]
        MOV     [R0],R10
        MOV     [R0+#02H],R11

        EXTP    R2,#01H
        MOV     R10,[R1]
        MOV     [R0+#04H],R10

        EXTP    R4,#02H
        MOV     R10,[R3]
        MOV     R11,[R3+#02H]
        MOV     [R0+#06H],R10
        MOV     [R0+#08H],R11

        EXTP    R6,#02H
        MOV     R10,[R5]
        MOV     R11,[R5+#02H]
        MOV     [R0+#0AH],R10
        MOV     [R0+#0CH],R11
        MOV     [R0+#0EH],R7
        CALLS   10h,1800h
        ADD     R0,#10H
        MOV     R7,[R0+]
        MOV     R6,[R0+]
        RETS
_llvm_layout_bit_edges2_abi_proxy ENDP

LLVM_PROXY_PR  ENDS

        EXTERN  _tasking_layout_packed_bits0:FAR
        REGDEF  R0-R15
        END
