#include "c166_test.h"
#include "types.h"

extern abi_u32 linked_application(abi_u16 seed);

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

  tap_plan(32U);
  for (index = 0; index != 8; ++index) {
    tap_is_u32(linked_application(seeds[index]),
               expected_result(seeds[index], &expected_value, &expected_state),
               "linked application result");
    tap_is_u32(callback_count, index + 1U, "callback count");
    tap_is_u32(callback_value, expected_value, "callback value");
    tap_is_u32(callback_state, expected_state, "callback state");
  }
}
