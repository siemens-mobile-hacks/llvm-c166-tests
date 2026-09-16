SECTIONS {
  .generated_cfg_selector 0xb0000 : AT(0xb0000) {
    *(.generated_cfg_selector)
  }
  .llvm_generated_cfg 0x100100 : AT(0x100100) {
    *(.llvm_generated_cfg)
  }
} INSERT AFTER .c166.regbank;
