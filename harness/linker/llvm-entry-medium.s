; Universal ABI-transparent near entry point for a Medium LLVM overlay.  The
; linker defines __c166_test_target to the case manifest's llvm_entry symbol.
.section .c166_test_medium_entry,"ax",@progbits
.globl __c166_test_medium_overlay_entry
.type __c166_test_medium_overlay_entry,@function
.c166_function near, __c166_test_medium_overlay_entry
__c166_test_medium_overlay_entry:
  jmpa cc_uc, cof(__c166_test_target)
.size __c166_test_medium_overlay_entry, .-__c166_test_medium_overlay_entry
