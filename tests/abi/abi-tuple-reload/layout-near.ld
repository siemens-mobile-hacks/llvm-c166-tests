SECTIONS {
  .abi.tuple-reload.selector 0x8100 : AT(0x8100) { *(.abi.tuple-reload.selector) }
  .abi.tuple-reload.low 0xc200 : AT(0xc200) { *(.abi.tuple-reload.low) }
  .abi.tuple-reload.high 0xd200 : AT(0xd200) { *(.abi.tuple-reload.high) }
  .c166.near.text 0x9000 : AT(0x9000) { *(.c166.near.text .c166.near.text.*) }
} INSERT BEFORE .rodata;
