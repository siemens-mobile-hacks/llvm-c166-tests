SECTIONS {
  .c166.near.text 0x9000 : AT(0x9000) { *(.c166.near.text .c166.near.text.*) }
  .abi.fp.float 0xb400 : AT(0xb400) { *(.abi.fp.float) }
  .abi.fp.double 0xbc00 : AT(0xbc00) { *(.abi.fp.double) }
  .abi.fp.reverse 0xc400 : AT(0xc400) { *(.abi.fp.reverse) }
} INSERT BEFORE .rodata;
