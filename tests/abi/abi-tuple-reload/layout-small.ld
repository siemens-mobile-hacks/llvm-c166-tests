SECTIONS {
  .abi.tuple-reload.selector 0x0b0000 : AT(0x0b0000) { *(.abi.tuple-reload.selector) }
  .abi.tuple-reload.low 0x100100 : AT(0x100100) { *(.abi.tuple-reload.low) }
  .abi.tuple-reload.high 0x101100 : AT(0x101100) { *(.abi.tuple-reload.high) }
} INSERT AFTER .c166.regbank;
