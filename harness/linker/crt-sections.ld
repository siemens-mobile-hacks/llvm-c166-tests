/*
 * Every writable C address space has its own output section, but startup is
 * model-independent.  llvm-crt.c walks one table containing these ranges.
 */
SECTIONS {
  .c166.near.bss 0x5800 (NOLOAD) : {
    _c166_near_bss_start = .;
    *(.c166.near.bss .c166.near.bss.*)
    _c166_near_bss_end = .;
  }
  .c166.xnear.bss 0x6800 (NOLOAD) : {
    _c166_xnear_bss_start = .;
    *(.c166.xnear.bss .c166.xnear.bss.*)
    _c166_xnear_bss_end = .;
  }
  .c166.small.bss 0x5800 (NOLOAD) : {
    _c166_small_bss_start = .;
    *(.c166.small.bss .c166.small.bss.*)
    _c166_small_bss_end = .;
  }
  .c166.small.far.bss 0x4c0800 (NOLOAD) : {
    _c166_far_bss_start = .;
    *(.c166.small.far.bss .c166.small.far.bss.*)
    _c166_far_bss_end = .;
  }
  .c166.small.huge.bss 0x4d0800 (NOLOAD) : {
    _c166_huge_bss_start = .;
    *(.c166.small.huge.bss .c166.small.huge.bss.*)
    _c166_huge_bss_end = .;
  }
  .c166.small.shuge.bss 0x4e0800 (NOLOAD) : {
    _c166_shuge_bss_start = .;
    *(.c166.small.shuge.bss .c166.small.shuge.bss.*)
    _c166_shuge_bss_end = .;
  }
  .bss 0x500000 (NOLOAD) : {
    _c166_default_bss_start = .;
    *(.bss .bss.* COMMON)
    _c166_default_bss_end = .;
  }
}
