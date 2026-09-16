SECTIONS {
  .abi.pressure.selector.alternate 0x098000 : AT(0x098000) { *(.abi.pressure.selector.alternate) }
  .abi.pressure.alternate.words 0x099000 : AT(0x099000) { *(.abi.pressure.alternate.words) }
  .abi.pressure.alternate.longs 0x09b000 : AT(0x09b000) { *(.abi.pressure.alternate.longs) }
  .abi.pressure.primary.words 0x100100 : AT(0x100100) { *(.abi.pressure.primary.words) }
  .abi.pressure.primary.longs 0x100900 : AT(0x100900) { *(.abi.pressure.primary.longs) }
  .abi.pressure.primary-alternate.words 0x101100 : AT(0x101100) { *(.abi.pressure.primary-alternate.words) }
  .abi.pressure.primary-alternate.longs 0x101900 : AT(0x101900) { *(.abi.pressure.primary-alternate.longs) }
  .abi.pressure.selector.primary 0x102100 : AT(0x102100) { *(.abi.pressure.selector.primary) }
  .abi.pressure.forward.words 0x102300 : AT(0x102300) { *(.abi.pressure.forward.words) }
  .abi.pressure.forward.longs 0x102900 : AT(0x102900) { *(.abi.pressure.forward.longs) }
} INSERT AFTER .c166.regbank;
