; Universal ABI-transparent entry point for an LLVM overlay.  The linker
; defines __c166_test_target to the case manifest's llvm_entry symbol.
.section .c166_test_entry,"ax",@progbits
.globl __c166_test_overlay_entry
.type __c166_test_overlay_entry,@function
.c166_function huge, __c166_test_overlay_entry
__c166_test_overlay_entry:
  jmps seg(__c166_test_target), sof(__c166_test_target)
.size __c166_test_overlay_entry, .-__c166_test_overlay_entry

; The runner calls this entry once before invoking the test entry.  It is a
; tail jump so c166_crt_init returns directly to the runner's proxy.
.section .c166_crt_entry,"ax",@progbits
.globl __c166_crt_overlay_entry
.type __c166_crt_overlay_entry,@function
.c166_function huge, __c166_crt_overlay_entry
__c166_crt_overlay_entry:
  jmps seg(_c166_crt_init), sof(_c166_crt_init)
.size __c166_crt_overlay_entry, .-__c166_crt_overlay_entry
