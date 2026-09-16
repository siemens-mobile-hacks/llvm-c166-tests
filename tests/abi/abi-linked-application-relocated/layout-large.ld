SECTIONS {
  .linked.text 0x1a0000 : AT(0x1a0000) { *(.linked.text) }
  .linked.data 0x420000 : AT(0x420000) { *(.linked.data) }
  .linked.rodata 0x424000 : AT(0x424000) { *(.linked.rodata) }
} INSERT AFTER .c166.regbank;
