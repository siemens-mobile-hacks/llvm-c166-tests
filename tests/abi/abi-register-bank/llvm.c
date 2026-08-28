typedef unsigned int u16;

volatile u16 interrupt_result
    __attribute__((section(".c166.interrupt.data")));

__attribute__((noinline))
u16 interrupt_mix(u16 a, u16 b, u16 c, u16 d, u16 e) {
  return (u16)(a * b + c * d + e);
}

__attribute__((interrupt(-1), c166_register_bank("ISS_BANK"),
               section(".c166.interrupt.text")))
void llvm_interrupt(void) {
  volatile u16 locals[4];
  locals[0] = 3;
  locals[1] = 5;
  locals[2] = 7;
  locals[3] = 11;
  interrupt_result =
      interrupt_mix(locals[0], locals[1], locals[2], locals[3], 13);
}

u16 llvm_anchor(void) { return 0; }
