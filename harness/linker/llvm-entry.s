; Universal ABI-transparent entry point for an LLVM overlay.  The linker
; defines __c166_test_target to the case manifest's llvm_entry symbol.
.section .c166_test_entry,"ax",@progbits
.globl __c166_test_overlay_entry
.type __c166_test_overlay_entry,@function
.c166_function huge, __c166_test_overlay_entry
__c166_test_overlay_entry:
  jmps seg(__c166_test_target), sof(__c166_test_target)
.size __c166_test_overlay_entry, .-__c166_test_overlay_entry
