#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 tasking_stack_page_limit_proxy(void);
extern abi_u16 llvm_stack_page_limit_proxy(void);
extern abi_u16 tasking_calls_llvm_stack_page_limit_proxy(void);
extern abi_u16 llvm_calls_tasking_stack_page_limit_proxy(void);
extern abi_u16 llvm_stack_escape_store_proxy(void);
extern abi_u16 asm_stack_escape_llvm_proxy(void);
extern abi_u16 asm_stack_escape_tasking_proxy(void);

void simulator_stop(void) {
  for (;;)
    ;
}

void main(void) {
  c166_test_begin(276, 0x16600114UL);
  c166_test_check_u32(1U, ABI_STACK_PAGE_LIMIT_GOLDEN,
                      tasking_stack_page_limit_proxy());
  c166_test_check_u32(2U, ABI_STACK_PAGE_LIMIT_GOLDEN,
                      llvm_stack_page_limit_proxy());
  c166_test_check_u32(3U, ABI_STACK_PAGE_LIMIT_GOLDEN,
                      tasking_calls_llvm_stack_page_limit_proxy());
  c166_test_check_u32(4U, ABI_STACK_PAGE_LIMIT_GOLDEN,
                      llvm_calls_tasking_stack_page_limit_proxy());
  c166_test_check_u32(5U, ABI_STACK_ESCAPE_LLVM_STORE_GOLDEN,
                      llvm_stack_escape_store_proxy());
  c166_test_check_u32(6U, ABI_STACK_ESCAPE_ASM_LLVM_GOLDEN,
                      asm_stack_escape_llvm_proxy());
  c166_test_check_u32(7U, ABI_STACK_ESCAPE_ASM_TASKING_GOLDEN,
                      asm_stack_escape_tasking_proxy());
  c166_test_finish();
  simulator_stop();
}

