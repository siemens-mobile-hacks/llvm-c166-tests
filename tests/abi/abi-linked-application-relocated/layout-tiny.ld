SECTIONS {
  .linked.text 0xe000 : AT(0xe000) { *(.linked.text) }
  .linked.data 0x6a00 : AT(0x6a00) { *(.linked.data) }
  .linked.rodata 0x6c00 : AT(0x6c00) { *(.linked.rodata) }
} INSERT AFTER .c166.regbank;
