SECTIONS {
  .abi.fp.float 0x100100 : AT(0x100100) { *(.abi.fp.float) }
  .abi.fp.double 0x100800 : AT(0x100800) { *(.abi.fp.double) }
  .abi.fp.reverse 0x101000 : AT(0x101000) { *(.abi.fp.reverse) }
} INSERT AFTER .c166.regbank;
