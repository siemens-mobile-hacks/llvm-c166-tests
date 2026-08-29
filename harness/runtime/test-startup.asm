$CASE
$GENONLY
$DEBUG
$NOLOCALS
$NOWARNING(120)
$CHECKCPU16
$CHECKBUS18
$INCLUDE(c166-asm-architecture.inc)
$INCLUDE(c166-asm-model.inc)
$INCLUDE(head.asm)
$INCLUDE(_c_init.asm)
; simulator.sh supplies MODEL(...) directly to a166.
@IF( ! @TASKING_MODEL_IS_SMALL )
        ASSUME DPP1:C166_XGROUP
        ASSUME DPP2:C166_DGROUP
@ENDI

        NAME    C166_TEST_STARTUP

C166_TEST_STARTUP_PR SECTION CODE WORD PUBLIC 'ASMPROG'
        PUBLIC __CSTART
        ; This is a frame-free tail jump.  The common runner supplies the same
        ; TASKING_MODEL_IS_MEDIUM value to every m166 input; a166 receives the
        ; corresponding MODEL(...) option separately.
@IF( @TASKING_MODEL_IS_MEDIUM )
        EXTERN _main:NEAR
@ELSE
        EXTERN _main:FAR
@ENDI

__CSTART PROC FAR
        MOV STKOV,#?SYSSTACK_BOTTOM + 0Ch
        MOV SP,#?SYSSTACK_TOP
        MOV STKUN,#?SYSSTACK_TOP
$NOCHECKPECCP
        MOV CP,#CSTART_RBANK
        NOP
@IF( @TASKING_MODEL_IS_SMALL )
        MOV DPP0,#PAG ?BASE_DPP0
        MOV DPP1,#PAG ?BASE_DPP1
        MOV DPP2,#PAG ?BASE_DPP2
        MOV R0,#?USRSTACK_BOTTOM + (?USRSTACK_TOP - ?USRSTACK_BOTTOM)
@ELSE
        MOV DPP1,#PAG C166_XGROUP
        MOV R0,#POF (?USRSTACK_TOP - 1) + 04001h
@ENDI
        DISWDT
@IF( ! @TASKING_MODEL_IS_SMALL )
        MOV DPP2,#PAG C166_DGROUP
@ENDI
        EINIT
        @_CALL( __C_INIT, R1 )
        ; CrossView's simulator cstart enables interrupts so its monitor can
        ; regain control.  The shared simulator startup must provide the same
        ; processor state even though it is assembled outside cc166's EVA
        ; driver path.
        BSET IEN
@IF( @TASKING_MODEL_IS_MEDIUM )
        JMPA    cc_UC,_main
@ELSE
        JMPS    SEG _main,_main
@ENDI
__CSTART ENDP
C166_TEST_STARTUP_PR ENDS

@IF( ! @TASKING_MODEL_IS_SMALL )
        C166_DGROUP DGROUP __C166_TEST_DUMMY
__C166_TEST_DUMMY SECTION DATA WORD PUBLIC 'CNEAR'
__C166_TEST_DUMMY ENDS

        C166_XGROUP DGROUP __C166_TEST_XDUMMY,C166_US
__C166_TEST_XDUMMY SECTION DATA WORD PUBLIC
__C166_TEST_XDUMMY ENDS
@ENDI
@IF( @TASKING_MODEL_IS_SMALL )
C166_US SECTION LDAT WORD GLBUSRSTACK 'CUSTACK'
@ELSE
C166_US SECTION DATA WORD GLBUSRSTACK 'CUSTACK'
@ENDI
        DS 02h
C166_US ENDS

CSTART_RBANK REGDEF R0-R15
        END
