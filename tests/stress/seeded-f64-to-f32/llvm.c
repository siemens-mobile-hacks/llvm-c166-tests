__attribute__((noinline, section(".llvm_f64_to_f32")))
float llvm_f64_to_f32(double value) { return (float)value; }
