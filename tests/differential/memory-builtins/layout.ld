SECTIONS {
  .memory.page_a 0x410800 : AT(0x410800) { *(.memory.page_a) }
  .memory.page_b 0x414800 : AT(0x414800) { *(.memory.page_b) }
} INSERT AFTER .c166.regbank;
