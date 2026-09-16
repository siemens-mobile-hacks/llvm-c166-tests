SECTIONS {
  .xnear_words 0x6000 (NOLOAD) : { *(.xnear_words) }
  .near_words 0x8800 (NOLOAD) : { *(.near_words) }
} INSERT AFTER .c166.regbank;
