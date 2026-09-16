SECTIONS {
  .far_page1 0x13fd0 (NOLOAD) : { *(.far_page1) }
  .far_page4 0x43fd0 (NOLOAD) : { *(.far_page4) }
} INSERT AFTER .c166.regbank;
