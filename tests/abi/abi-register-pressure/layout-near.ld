SECTIONS {
  .abi.pressure.selector.alternate 0x8100 : AT(0x8100) { *(.abi.pressure.selector.alternate) }
  .abi.pressure.alternate.words 0x8300 : AT(0x8300) { *(.abi.pressure.alternate.words) }
  .abi.pressure.alternate.longs 0x8700 : AT(0x8700) { *(.abi.pressure.alternate.longs) }
  .c166.near.text 0x9000 : AT(0x9000) { *(.c166.near.text .c166.near.text.*) }
  .abi.pressure.primary.words 0xc200 : AT(0xc200) { *(.abi.pressure.primary.words) }
  .abi.pressure.primary.longs 0xca00 : AT(0xca00) { *(.abi.pressure.primary.longs) }
  .abi.pressure.primary-alternate.words 0xd200 : AT(0xd200) { *(.abi.pressure.primary-alternate.words) }
  .abi.pressure.primary-alternate.longs 0xda00 : AT(0xda00) { *(.abi.pressure.primary-alternate.longs) }
  .abi.pressure.selector.primary 0xe200 : AT(0xe200) { *(.abi.pressure.selector.primary) }
  .abi.pressure.forward.words 0xe400 : AT(0xe400) { *(.abi.pressure.forward.words) }
  .abi.pressure.forward.longs 0xea00 : AT(0xea00) { *(.abi.pressure.forward.longs) }
} INSERT BEFORE .rodata;
