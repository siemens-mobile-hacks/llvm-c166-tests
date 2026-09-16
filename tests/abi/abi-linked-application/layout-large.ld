SECTIONS {
  .linked.text 0x190000 : AT(0x190000) { *(.linked.text) }
  .linked.data 0x410000 : AT(0x410000) { *(.linked.data) }
  .linked.rodata 0x414000 : AT(0x414000) { *(.linked.rodata) }
} INSERT AFTER .c166.regbank;
