#include "types.h"

volatile abi_double_words tasking_double_observed;

double tasking_double_identity(double value)
{
  tasking_double_observed.value = value;
  return value;
}
