#include "c166-test-result.h"
#include "c166-test-runtime.h"
#include "types.h"

extern abi_u32 llvm_entry_proxy(abi_u16 seed);

static abi_u16 callback_count;
static abi_u16 callback_value;
static abi_u16 callback_state;

abi_u16 linked_host_callback(abi_u16 value, abi_u16 state) {
  ++callback_count;
  callback_value = value;
  callback_state = state;
  return linked_callback_expected(value, state);
}

static abi_u32 expected_result(abi_u16 seed, abi_u16 *value_before_callback,
                               abi_u16 *final_state) {
  abi_u16 state = linked_initial_state(seed);
  abi_u16 value = (abi_u16)(seed ^ LINKED_MARKER);

  value = linked_archive_expected(value);
  value = linked_selected_expected(value);
  state = linked_common_expected(state, value);
  value = linked_observe_expected(state, state);
  *value_before_callback = value;
  *final_state = state;
  return ((abi_u32)state << 16) | linked_callback_expected(value, state);
}

void main(void) {
  static const abi_u16 seeds[8] = {
      0x0000U, 0x0001U, 0x1234U, 0x3fffU,
      0x8000U, 0xa55aU, 0xfffeU, 0xffffU,
  };
  abi_u16 expected_value;
  abi_u16 expected_state;
  abi_u16 index;
  abi_u16 case_id = 1;
  abi_u32 expected;
  abi_u32 actual;

  c166_test_begin(386, 0x16600182UL);
  for (index = 0; index != 8; ++index) {
    expected = expected_result(seeds[index], &expected_value, &expected_state);
    actual = llvm_entry_proxy(seeds[index]);
    c166_test_check_u32(case_id++, expected, actual);
    c166_test_check_u32(case_id++, index + 1U, callback_count);
    c166_test_check_u32(case_id++, expected_value, callback_value);
    c166_test_check_u32(case_id++, expected_state, callback_state);
  }
  c166_test_finish();
  simulator_stop();
}
