#include "types.h"

#define NOINLINE __attribute__((noinline))

extern abi_u16 ABI_STACKPARM tasking_stackparm_callee(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d);
extern float ABI_STACKPARM tasking_stackparm_float_callee(float value,
                                                          abi_u16 tail);
extern double ABI_STACKPARM tasking_stackparm_double_callee(double value,
                                                            abi_u16 tail);

NOINLINE __attribute__((section(".llvm_stackparm_callee")))
abi_u16 ABI_STACKPARM llvm_stackparm_callee(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d) {
  return (abi_u16)((abi_u16)(a * 19U) + b +
                   (abi_u16)((abi_u16)c * 3U) +
                   (abi_u16)((abi_u16)(c >> 16) * 7U) +
                   (abi_u16)(*p * 5U) + pair.lo +
                   (abi_u16)(pair.hi * 11U) + (abi_u16)(d * 13U));
}

NOINLINE __attribute__((section(".llvm_stackparm_float_callee")))
float ABI_STACKPARM llvm_stackparm_float_callee(float value, abi_u16 tail) {
  (void)tail;
  return value;
}

NOINLINE __attribute__((section(".llvm_stackparm_double_callee")))
double ABI_STACKPARM llvm_stackparm_double_callee(double value, abi_u16 tail) {
  (void)tail;
  return value;
}

NOINLINE __attribute__((section(".llvm_stackparm_special_routes")))
void llvm_stackparm_special_routes(
    abi_stackparm_float_function *float_function,
    abi_stackparm_double_function *double_function,
    const abi_float_words _far *float_input,
    const abi_double_words _far *double_input,
    abi_float_words _far *float_output,
    abi_double_words _far *double_output, abi_u16 tail) {
  float_output[0].value =
      tasking_stackparm_float_callee(float_input->value, tail);
  float_output[1].value = float_function(float_input->value, tail);
  double_output[0].value =
      tasking_stackparm_double_callee(double_input->value, tail);
  double_output[1].value = double_function(double_input->value, tail);
}

NOINLINE abi_u32 llvm_entry(
    abi_stackparm_function *function, abi_u8 a, abi_u16 b, abi_u32 c,
    const abi_u16 _far *p, struct abi_stackparm_pair pair, abi_u16 d) {
  abi_u16 direct = tasking_stackparm_callee(a, b, c, p, pair, d);
  abi_u16 indirect = function(a, b, c, p, pair, d);
  return (abi_u32)direct | ((abi_u32)indirect << 16);
}
