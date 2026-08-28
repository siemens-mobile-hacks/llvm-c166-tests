$EXTEND
$NOMOD166
$STDNAMES(reg.def)
$SEGMENTED
$CASE
$NOEXPANDREGBANK
$MODEL(LARGE)
        ASSUME DPP2:C166_DGROUP

        NAME    FLOAT64_TASKING_REVERSE_ABI_BOUNDARY

LLVM_PROXY_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC _llvm_tasking_double_identity_proxy
        PUBLIC __load8n
        PUBLIC __ldnoal8n
        PUBLIC __store8n
        PUBLIC __load8f
        PUBLIC __ldnoal8f
        PUBLIC __store8f
        PUBLIC __cpnnb
        PUBLIC __cpnnw
        PUBLIC __cpnfb
        PUBLIC __cpnfw
        PUBLIC __cpfnb
        PUBLIC __cpfnw
        PUBLIC _tasking_helper_load_store_near
        PUBLIC _tasking_helper_load_store_far
        PUBLIC _tasking_helper_ldnoal_near
        PUBLIC _tasking_helper_ldnoal_far
        PUBLIC _tasking_helper_cpnnb
        PUBLIC _tasking_helper_cpnnw
        PUBLIC _tasking_helper_cpnfb
        PUBLIC _tasking_helper_cpnfw
        PUBLIC _tasking_helper_cpfnb
        PUBLIC _tasking_helper_cpfnw

_llvm_tasking_double_identity_proxy PROC FAR
        CALLS 10h,00100h
        RETS
_llvm_tasking_double_identity_proxy ENDP

; TASKING code resolves its Classic runtime symbols here.  Every entry is a
; tail jump to a fixed ELF veneer, which then relocates to the corresponding
; implementation pulled from compiler-rt.  No TASKING helper implementation is
; linked into this test image.
__load8n PROC FAR
        JMPS 10h,00500h
__load8n ENDP

__ldnoal8n PROC FAR
        JMPS 10h,00504h
__ldnoal8n ENDP

__store8n PROC FAR
        JMPS 10h,00508h
__store8n ENDP

__load8f PROC FAR
        JMPS 10h,0050Ch
__load8f ENDP

__ldnoal8f PROC FAR
        JMPS 10h,00510h
__ldnoal8f ENDP

__store8f PROC FAR
        JMPS 10h,00514h
__store8f ENDP

__cpnnb PROC FAR
        JMPS 10h,00518h
__cpnnb ENDP

__cpnnw PROC FAR
        JMPS 10h,0051Ch
__cpnnw ENDP

__cpnfb PROC FAR
        JMPS 10h,00520h
__cpnfb ENDP

__cpnfw PROC FAR
        JMPS 10h,00524h
__cpnfw ENDP

__cpfnb PROC FAR
        JMPS 10h,00528h
__cpfnb ENDP

__cpfnw PROC FAR
        JMPS 10h,0052Ch
__cpfnw ENDP

_tasking_helper_load_store_near PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        CALLS SEG __load8n,__load8n
        MOV R4,#DPP2:_tasking_near_destination
        CALLS SEG __store8n,__store8n
        ADD R0,#08h
        RETS
_tasking_helper_load_store_near ENDP

_tasking_helper_load_store_far PROC FAR
        MOV R4,#POF _tasking_far_source
        MOV R5,#PAG _tasking_far_source
        CALLS SEG __load8f,__load8f
        MOV R4,#POF _tasking_far_destination
        MOV R5,#PAG _tasking_far_destination
        CALLS SEG __store8f,__store8f
        ADD R0,#08h
        RETS
_tasking_helper_load_store_far ENDP

_tasking_helper_ldnoal_near PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        MOV R10,#DPP2:_tasking_near_destination
        CALLS SEG __ldnoal8n,__ldnoal8n
        RETS
_tasking_helper_ldnoal_near ENDP

_tasking_helper_ldnoal_far PROC FAR
        MOV R4,#POF _tasking_far_source
        MOV R5,#PAG _tasking_far_source
        MOV R10,#DPP2:_tasking_near_destination
        CALLS SEG __ldnoal8f,__ldnoal8f
        RETS
_tasking_helper_ldnoal_far ENDP

_tasking_helper_cpnnb PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        MOV R10,#DPP2:_tasking_near_destination
        MOV R3,#08h
        CALLS SEG __cpnnb,__cpnnb
        RETS
_tasking_helper_cpnnb ENDP

_tasking_helper_cpnnw PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        MOV R10,#DPP2:_tasking_near_destination
        MOV R3,#04h
        CALLS SEG __cpnnw,__cpnnw
        RETS
_tasking_helper_cpnnw ENDP

_tasking_helper_cpnfb PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        MOV R10,#POF _tasking_far_destination
        MOV R11,#PAG _tasking_far_destination
        MOV R3,#08h
        CALLS SEG __cpnfb,__cpnfb
        RETS
_tasking_helper_cpnfb ENDP

_tasking_helper_cpnfw PROC FAR
        MOV R4,#DPP2:_tasking_near_source
        MOV R10,#POF _tasking_far_destination
        MOV R11,#PAG _tasking_far_destination
        MOV R3,#04h
        CALLS SEG __cpnfw,__cpnfw
        RETS
_tasking_helper_cpnfw ENDP

_tasking_helper_cpfnb PROC FAR
        MOV R4,#POF _tasking_far_source
        MOV R5,#PAG _tasking_far_source
        MOV R10,#DPP2:_tasking_near_destination
        MOV R3,#08h
        CALLS SEG __cpfnb,__cpfnb
        RETS
_tasking_helper_cpfnb ENDP

_tasking_helper_cpfnw PROC FAR
        MOV R4,#POF _tasking_far_source
        MOV R5,#PAG _tasking_far_source
        MOV R10,#DPP2:_tasking_near_destination
        MOV R3,#04h
        CALLS SEG __cpfnw,__cpfnw
        RETS
_tasking_helper_cpfnw ENDP
LLVM_PROXY_PR ENDS

TASKING_HELPER_NEAR_DATA SECTION DATA WORD PUBLIC 'CNEAR'
_tasking_near_source LABEL WORD
        DS 08h
        PUBLIC _tasking_near_source
_tasking_near_destination LABEL WORD
        DS 08h
        PUBLIC _tasking_near_destination
TASKING_HELPER_NEAR_DATA ENDS

TASKING_HELPER_FAR_DATA SECTION DATA WORD PUBLIC 'CFAR'
_tasking_far_source LABEL WORD
        DS 08h
        PUBLIC _tasking_far_source
_tasking_far_destination LABEL WORD
        DS 08h
        PUBLIC _tasking_far_destination
TASKING_HELPER_FAR_DATA ENDS

        C166_DGROUP DGROUP TASKING_HELPER_NEAR_DATA
        REGDEF R0-R15
        END
