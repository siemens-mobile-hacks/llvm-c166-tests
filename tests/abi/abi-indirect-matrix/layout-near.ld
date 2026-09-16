SECTIONS {
  .abi.indirect.target0 0xc200 : AT(0xc200) { *(.abi.indirect.target0) }
  .abi.indirect.target2 0xc280 : AT(0xc280) { *(.abi.indirect.target2) }
  .abi.indirect.target5 0xc380 : AT(0xc380) { *(.abi.indirect.target5) }
  .abi.indirect.call0 0xc500 : AT(0xc500) { *(.abi.indirect.call0) }
  .abi.indirect.call2 0xc700 : AT(0xc700) { *(.abi.indirect.call2) }
  .abi.indirect.call5 0xca00 : AT(0xca00) { *(.abi.indirect.call5) }
  .abi.indirect.get0 0xce00 : AT(0xce00) { *(.abi.indirect.get0) }
  .abi.indirect.get2 0xce80 : AT(0xce80) { *(.abi.indirect.get2) }
  .abi.indirect.get5 0xcf00 : AT(0xcf00) { *(.abi.indirect.get5) }
} INSERT BEFORE .rodata;
