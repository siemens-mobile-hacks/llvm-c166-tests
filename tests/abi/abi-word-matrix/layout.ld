SECTIONS {
  .abi.word0 0x120000 : AT(0x120000) { *(.abi.word0) }
  .abi.word1 0x121000 : AT(0x121000) { *(.abi.word1) }
  .abi.word2 0x122000 : AT(0x122000) { *(.abi.word2) }
  .abi.word3 0x123000 : AT(0x123000) { *(.abi.word3) }
  .abi.word4 0x124000 : AT(0x124000) { *(.abi.word4) }
  .abi.word5 0x125000 : AT(0x125000) { *(.abi.word5) }
  .abi.word6 0x126000 : AT(0x126000) { *(.abi.word6) }
  .abi.word7 0x127000 : AT(0x127000) { *(.abi.word7) }
  .abi.word8 0x128000 : AT(0x128000) { *(.abi.word8) }
  .abi.word9 0x129000 : AT(0x129000) { *(.abi.word9) }
  .abi.word10 0x12a000 : AT(0x12a000) { *(.abi.word10) }
  .abi.word.reverse 0x12b000 : AT(0x12b000) { *(.abi.word.reverse) }
} INSERT BEFORE .rodata;
