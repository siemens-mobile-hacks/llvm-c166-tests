#include "types.h"

__attribute__((noinline))
void llvm_entry(const struct abi_pointer_cast_input *input,
                struct abi_pointer_cast_output *output) {
  output->data_to_long = (abi_u32)input->data;
  output->long_to_data = (volatile abi_u16 *)input->linear;
  output->data_roundtrip =
      (abi_u32)(volatile abi_u16 *)input->linear;
  output->function_to_long = (abi_u32)input->function;
  output->long_to_function = (abi_callback)input->linear;
}
