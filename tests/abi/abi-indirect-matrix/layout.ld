SECTIONS {
  .abi.indirect.target0 0x100100 : AT(0x100100) { *(.abi.indirect.target0) }
  .abi.indirect.target2 0x100180 : AT(0x100180) { *(.abi.indirect.target2) }
  .abi.indirect.target5 0x100280 : AT(0x100280) { *(.abi.indirect.target5) }
  .abi.indirect.call0 0x100400 : AT(0x100400) { *(.abi.indirect.call0) }
  .abi.indirect.call2 0x100600 : AT(0x100600) { *(.abi.indirect.call2) }
  .abi.indirect.call5 0x100900 : AT(0x100900) { *(.abi.indirect.call5) }
  .abi.indirect.get0 0x100d00 : AT(0x100d00) { *(.abi.indirect.get0) }
  .abi.indirect.get2 0x100d80 : AT(0x100d80) { *(.abi.indirect.get2) }
  .abi.indirect.get5 0x100e00 : AT(0x100e00) { *(.abi.indirect.get5) }
} INSERT BEFORE .rodata;
