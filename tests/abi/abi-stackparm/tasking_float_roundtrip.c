#include "types.h"

float ABI_STACKPARM tasking_stackparm_float_callee(float value, abi_u16 tail)
{
  (void)tail;
  return value;
}
