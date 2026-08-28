; Canonical TASKING assembler address mode for the selected Classic memory
; model.  simulator.sh defines TASKING_MODEL_IS_SMALL before m166 runs.
@IF( @TASKING_MODEL_IS_SMALL )
$NONSEGMENTED
@ELSE
$SEGMENTED
@ENDI
