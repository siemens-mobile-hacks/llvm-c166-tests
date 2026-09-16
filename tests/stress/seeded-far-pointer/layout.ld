SECTIONS {
  .seeded_far_arena 0x404000 (NOLOAD) : {
    KEEP(*(.seeded_far_arena))
  }
} INSERT AFTER .bss;
