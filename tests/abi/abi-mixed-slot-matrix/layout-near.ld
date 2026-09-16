SECTIONS {
  .c166.near.text 0x2000 : { *(.c166.near.text) }
  .abi_mixed0 0x4000 : { *(.abi_mixed0) }
  .abi_mixed1 0x4100 : { *(.abi_mixed1) }
  .abi_mixed2 0x4200 : { *(.abi_mixed2) }
  .abi_mixed3 0x4300 : { *(.abi_mixed3) }
  .abi_mixed4 0x4400 : { *(.abi_mixed4) }
  .abi_mixed5 0x4500 : { *(.abi_mixed5) }
  .abi_mixed6 0x4600 : { *(.abi_mixed6) }
  .abi_mixed7 0x4700 : { *(.abi_mixed7) }
  .abi_mixed8 0x4800 : { *(.abi_mixed8) }
  .abi_mixed9 0x4900 : { *(.abi_mixed9) }
  .abi_mixed10 0x4a00 : { *(.abi_mixed10) }
  .abi_mixed11 0x4b00 : { *(.abi_mixed11) }
  .abi_mixed12 0x4c00 : { *(.abi_mixed12) }
  .abi_mixed13 0x4d00 : { *(.abi_mixed13) }
  .abi_mixed14 0x4e00 : { *(.abi_mixed14) }
  .abi_mixed15 0x4f00 : { *(.abi_mixed15) }
  .abi_mixed16 0x5000 : { *(.abi_mixed16) }
  .abi_mixed17 0x5100 : { *(.abi_mixed17) }
  .abi_mixed18 0x5200 : { *(.abi_mixed18) }
  .abi_mixed19 0x5300 : { *(.abi_mixed19) }
  .abi_mixed20 0x5400 : { *(.abi_mixed20) }
  .abi_mixed21 0x5500 : { *(.abi_mixed21) }
  .abi_mixed_dispatch 0x5800 : { *(.abi_mixed_dispatch) }
  .abi_mixed_enum5 0x6000 : { *(.abi_mixed_enum5) }
  .abi_mixed_enum5_dispatch 0x6100 : { *(.abi_mixed_enum5_dispatch) }
} INSERT AFTER .text;
