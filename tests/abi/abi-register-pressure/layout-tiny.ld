SECTIONS {
  .abi.pressure.selector.alternate 0x2000 : AT(0x2000) { *(.abi.pressure.selector.alternate) }
  .abi.pressure.alternate.words 0x2200 : AT(0x2200) { *(.abi.pressure.alternate.words) }
  .abi.pressure.alternate.longs 0x2600 : AT(0x2600) { *(.abi.pressure.alternate.longs) }
  .abi.pressure.primary.words 0x3000 : AT(0x3000) { *(.abi.pressure.primary.words) }
  .abi.pressure.primary.longs 0x3800 : AT(0x3800) { *(.abi.pressure.primary.longs) }
  .abi.pressure.primary-alternate.words 0x4000 : AT(0x4000) { *(.abi.pressure.primary-alternate.words) }
  .abi.pressure.primary-alternate.longs 0x4800 : AT(0x4800) { *(.abi.pressure.primary-alternate.longs) }
  .abi.pressure.selector.primary 0x5000 : AT(0x5000) { *(.abi.pressure.selector.primary) }
  .abi.pressure.forward.words 0x5200 : AT(0x5200) { *(.abi.pressure.forward.words) }
  .abi.pressure.forward.longs 0x5800 : AT(0x5800) { *(.abi.pressure.forward.longs) }
  .c166.near.text 0x5a00 : AT(0x5a00) { *(.c166.near.text .c166.near.text.*) }
} INSERT BEFORE .rodata;
