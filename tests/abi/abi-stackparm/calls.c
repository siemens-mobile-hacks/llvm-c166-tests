#include "types.h"

C166_NOINLINE
abi_u16 stackparm_direct(abi_u8 a, abi_u16 b, abi_u32 c,
                         const abi_u16 C166_FAR *p,
                         struct abi_stackparm_pair pair, abi_u16 d) {
  return stackparm_callee(a, b, c, p, pair, d);
}

C166_NOINLINE
abi_u16 stackparm_indirect(abi_stackparm_function *function, abi_u8 a,
                           abi_u16 b, abi_u32 c,
                           const abi_u16 C166_FAR *p,
                           struct abi_stackparm_pair pair, abi_u16 d) {
  return function(a, b, c, p, pair, d);
}

C166_NOINLINE
float stackparm_float_direct(float value, abi_u16 tail) {
  return stackparm_float_callee(value, tail);
}

C166_NOINLINE
float stackparm_float_indirect(abi_stackparm_float_function *function,
                               float value, abi_u16 tail) {
  return function(value, tail);
}

C166_NOINLINE
double stackparm_double_direct(double value, abi_u16 tail) {
  return stackparm_double_callee(value, tail);
}

C166_NOINLINE
double stackparm_double_indirect(abi_stackparm_double_function *function,
                                 double value, abi_u16 tail) {
  return function(value, tail);
}

C166_NOINLINE C166_SECTION(".abi.stackparm.routes")
void stackparm_routes(
    abi_stackparm_float_function *float_function,
    abi_stackparm_double_function *double_function,
    const abi_float_words C166_FAR *float_input,
    const abi_double_words C166_FAR *double_input,
    abi_float_words C166_FAR *float_output,
    abi_double_words C166_FAR *double_output, abi_u16 tail) {
  float_output[0].value = stackparm_float_callee(float_input->value, tail);
  float_output[1].value = float_function(float_input->value, tail);
  double_output[0].value = stackparm_double_callee(double_input->value, tail);
  double_output[1].value = double_function(double_input->value, tail);
}
