#include "c166-test-runtime.h"
#include "c166-test-result.h"

extern unsigned int llvm_entry_proxy(unsigned int case_id);

void main(void) {
  unsigned int case_id, expected, actual;
  c166_test_begin(368, 0x166UL);
  for (case_id = 0; case_id != 11; ++case_id) {
    expected = (case_id == 8 || case_id == 10) ? 2 : 1;
    actual = llvm_entry_proxy(case_id);
    c166_test_check_u32(case_id + 1, expected, actual);
    if (actual != expected) break;
  }
  c166_test_finish();
  simulator_stop();
}
