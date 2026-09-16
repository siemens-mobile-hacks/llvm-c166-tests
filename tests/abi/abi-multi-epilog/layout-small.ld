SECTIONS {
  .abi.multi-epilog.selector 0x0b0000 : AT(0x0b0000) {
    *(.abi.multi-epilog.selector)
  }
  .abi.multi-epilog.pressure 0x100100 : AT(0x100100) {
    *(.abi.multi-epilog.pressure)
  }
} INSERT AFTER .c166.regbank;
