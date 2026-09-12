#include "c166-test-result.h"
#include "c166-test-runtime.h"

typedef unsigned int u16;
extern u16 llvm_entry_proxy(u16 input);

void main(void) {
  c166_test_begin(322, 0);
  c166_test_check_u32(1, 0x3f, llvm_entry_proxy(0));
  c166_test_check_u32(2, 0x3f, llvm_entry_proxy(1));
  c166_test_check_u32(3, 0x3f, llvm_entry_proxy(2));
  c166_test_check_u32(4, 0x3f, llvm_entry_proxy(0xffff));
  c166_test_finish();
  simulator_stop();
}
