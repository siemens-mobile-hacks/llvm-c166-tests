extern double tasking_double_identity(double value);

extern volatile double llvm_tasking_double_observed;

__attribute__((noinline, section(".llvm_tasking_double_identity")))
double llvm_tasking_double_identity(double value)
{
  double result = tasking_double_identity(value);
  llvm_tasking_double_observed = result;
  return result;
}
