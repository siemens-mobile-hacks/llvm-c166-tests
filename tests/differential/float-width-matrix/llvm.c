__attribute__((noinline, section(".llvm_f32_to_f64")))
double llvm_f32_to_f64(float value) { return (double)value; }

__attribute__((noinline, section(".llvm_f64_to_f32")))
float llvm_f64_to_f32(double value) { return (float)value; }
