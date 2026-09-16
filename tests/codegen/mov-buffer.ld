SECTIONS {
  .mov.buffer 0x420000 : AT(0x420000) { *(.mov.buffer) }
} INSERT AFTER .c166.regbank;
