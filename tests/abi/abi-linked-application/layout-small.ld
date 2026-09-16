SECTIONS {
  .linked.text 0x190000 : AT(0x190000) { *(.linked.text) }
  .linked.data 0x5a00 : AT(0x5a00) { *(.linked.data) }
  .linked.rodata 0x5c00 : AT(0x5c00) { *(.linked.rodata) }
} INSERT AFTER .c166.regbank;
