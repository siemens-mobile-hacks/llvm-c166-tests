#include "types.h"

double ABI_STACKPARM tasking_stackparm_double_callee(double value, abi_u16 tail)
{
  (void)tail;
  return value;
}
