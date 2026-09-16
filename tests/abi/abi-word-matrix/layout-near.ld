SECTIONS {
  .abi.word0 0xc100 : AT(0xc100) { *(.abi.word0) }
  .abi.word1 0xc120 : AT(0xc120) { *(.abi.word1) }
  .abi.word2 0xc1a0 : AT(0xc1a0) { *(.abi.word2) }
  .abi.word3 0xc280 : AT(0xc280) { *(.abi.word3) }
  .abi.word4 0xc3c0 : AT(0xc3c0) { *(.abi.word4) }
  .abi.word5 0xc560 : AT(0xc560) { *(.abi.word5) }
  .abi.word6 0xc760 : AT(0xc760) { *(.abi.word6) }
  .abi.word7 0xc9c0 : AT(0xc9c0) { *(.abi.word7) }
  .abi.word8 0xcc80 : AT(0xcc80) { *(.abi.word8) }
  .abi.word9 0xcfe0 : AT(0xcfe0) { *(.abi.word9) }
  .abi.word10 0xd3c0 : AT(0xd3c0) { *(.abi.word10) }
  .abi.word.reverse 0xd800 : AT(0xd800) { *(.abi.word.reverse) }
  .c166.near.text 0xdb00 : AT(0xdb00) {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT BEFORE .rodata;
