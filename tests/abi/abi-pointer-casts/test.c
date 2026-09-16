#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

C166_NOINLINE void
pointer_cast_kernel(const struct abi_pointer_cast_input *input,
                    struct abi_pointer_cast_output *output) {
  output->data_to_long = (abi_u32)input->data;
  output->long_to_data = (volatile abi_u16 *)input->linear;
  output->data_roundtrip = (abi_u32)(volatile abi_u16 *)input->linear;
  output->function_to_long = (abi_u32)input->function;
  output->long_to_function = (abi_callback)input->linear;
}

#if C166_TEST_SMALL
static abi_u16 dpp_page(abi_u16 selector) {
  switch (selector) {
  case 0:
    return C166_DPP0_VALUE;
  case 1:
    return C166_DPP1_VALUE;
  case 2:
    return C166_DPP2_VALUE;
  default:
    return C166_DPP3_VALUE;
  }
}
#endif

static abi_u32 expected_data_address(abi_u32 input,
                                     abi_u32 far_address) {
#if C166_TEST_SMALL
  abi_u16 direct = (abi_u16)input;
  return ((abi_u32)dpp_page(direct >> 14) << 14) |
         (direct & 0x3fffU);
#else
  (void)input;
  return far_address;
#endif
}

static void run_pointer_cast_vector(abi_u32 input_value,
                                    abi_u32 far_address) {
  struct abi_pointer_cast_input input;
  struct abi_pointer_cast_output output;
  abi_u32 data_address = expected_data_address(input_value, far_address);

  input.linear = input_value;
  input.data = (volatile abi_u16 *)input_value;
  input.function = (abi_callback)input_value;
  pointer_cast_kernel(&input, &output);

  tap_is_u32(output.data_to_long, data_address, "data pointer to integer");
  tap_is_u32((abi_u32)output.long_to_data, data_address,
             "integer to data pointer");
  tap_is_u32(output.data_roundtrip, data_address, "data pointer roundtrip");
  tap_is_u32(output.function_to_long, input_value,
             "function pointer to integer");
  tap_is_u32((abi_u32)output.long_to_function, input_value,
             "integer to function pointer");
}

#define RUN_POINTER_CAST(id, input, normalized)                              \
  run_pointer_cast_vector(input, normalized);

void main(void) {
  tap_plan(50U);
  ABI_POINTER_CAST_VECTORS(RUN_POINTER_CAST)
}
