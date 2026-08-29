#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern void llvm_entry_proxy(const struct abi_pointer_cast_input *input,
                             struct abi_pointer_cast_output *output);
extern void c166_test_entry(
    const struct abi_pointer_cast_input *input,
    struct abi_pointer_cast_output *output);

static void run_pointer_cast_vector(abi_u16 vector_id, abi_u32 input_value,
                                    abi_u32 normalized_data) {
  struct abi_pointer_cast_input input;
  struct abi_pointer_cast_output reference;
  struct abi_pointer_cast_output actual;
  abi_u16 case_base = (vector_id - 1U) * 10U;

#if TASKING_MODEL_IS_SMALL
  {
    /* layout.ilo fixes the simulator's DPP0..DPP3 pages. */
    static const abi_u16 dpp_pages[] = {0U, 1U, 4U, 3U};
    abi_u16 direct = (abi_u16)input_value;
    normalized_data =
        ((abi_u32)dpp_pages[direct >> 14] << 14) | (direct & 0x3fffU);
  }
#endif

  input.linear = input_value;
  input.data = (volatile abi_u16 *)input_value;
  input.function = (abi_callback)input_value;

  c166_test_entry(&input, &reference);
  llvm_entry_proxy(&input, &actual);

  c166_test_check_u32(case_base + 1U, normalized_data,
                      reference.data_to_long);
  c166_test_check_u32(case_base + 2U, normalized_data, actual.data_to_long);
  c166_test_check_u32(case_base + 3U, normalized_data,
                      (abi_u32)reference.long_to_data);
  c166_test_check_u32(case_base + 4U, normalized_data,
                      (abi_u32)actual.long_to_data);
  c166_test_check_u32(case_base + 5U, normalized_data,
                      reference.data_roundtrip);
  c166_test_check_u32(case_base + 6U, normalized_data,
                      actual.data_roundtrip);
  c166_test_check_u32(case_base + 7U, input_value,
                      reference.function_to_long);
  c166_test_check_u32(case_base + 8U, input_value,
                      actual.function_to_long);
  c166_test_check_u32(case_base + 9U, input_value,
                      (abi_u32)reference.long_to_function);
  c166_test_check_u32(case_base + 10U, input_value,
                      (abi_u32)actual.long_to_function);
}

#define RUN_POINTER_CAST(id, input, normalized) \
  run_pointer_cast_vector(id, input, normalized);

void main(void) {
  c166_test_begin(303, 0x1660011fUL);
  ABI_POINTER_CAST_VECTORS(RUN_POINTER_CAST)
  c166_test_finish();
  simulator_stop();
}
