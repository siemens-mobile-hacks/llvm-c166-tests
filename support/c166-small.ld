ENTRY(_reset)

SECTIONS {
  .reset 0 : { KEEP(*(.reset)) }
  .c166.halt 0x0fe0 : { KEEP(*(.c166.halt)) }
  .text 0x1000 : { *(.text.start) *(.text .text.*) }
  .rodata : {
    *(.rodata .rodata.*)
    *(.c166.small.rodata .c166.small.rodata.*)
  }
  ASSERT(. <= 0x8000, "near code and read-only data exceed their region")
  .data 0x8000 : AT(ALIGN(LOADADDR(.rodata) + SIZEOF(.rodata), 2)) {
    _c166_data_start = .;
    *(.data .data.*)
    *(.c166.small.data .c166.small.data.*)
    _c166_data_end = .;
  }
  _c166_data_load_start = LOADADDR(.data);
  .bss (NOLOAD) : {
    _c166_bss_start = .;
    *(.bss .bss.* COMMON)
    *(.c166.small.bss .c166.small.bss.*)
    _c166_bss_end = .;
  }
  .noinit (NOLOAD) : { *(.noinit .noinit.*) }
  ASSERT(. <= 0xa000, "near data overlaps the user stack")
  .c166.small.far.data 0x4c0000 :
      AT(ALIGN(LOADADDR(.data) + SIZEOF(.data), 2)) {
    _c166_far_data_start = .;
    *(.c166.small.far.data .c166.small.far.data.*)
    _c166_far_data_end = .;
  }
  _c166_far_data_load_start = LOADADDR(.c166.small.far.data);
  .c166.small.far.bss 0x4c0800 (NOLOAD) : {
    _c166_far_bss_start = .;
    *(.c166.small.far.bss .c166.small.far.bss.*)
    _c166_far_bss_end = .;
  }
  .c166.small.huge.data 0x4d0000 :
      AT(ALIGN(LOADADDR(.c166.small.far.data) +
               SIZEOF(.c166.small.far.data), 2)) {
    _c166_huge_data_start = .;
    *(.c166.small.huge.data .c166.small.huge.data.*)
    _c166_huge_data_end = .;
  }
  _c166_huge_data_load_start = LOADADDR(.c166.small.huge.data);
  .c166.small.huge.bss 0x4d0800 (NOLOAD) : {
    _c166_huge_bss_start = .;
    *(.c166.small.huge.bss .c166.small.huge.bss.*)
    _c166_huge_bss_end = .;
  }
  .c166.small.shuge.data 0x4e0000 :
      AT(ALIGN(LOADADDR(.c166.small.huge.data) +
               SIZEOF(.c166.small.huge.data), 2)) {
    _c166_shuge_data_start = .;
    *(.c166.small.shuge.data .c166.small.shuge.data.*)
    _c166_shuge_data_end = .;
  }
  _c166_shuge_data_load_start = LOADADDR(.c166.small.shuge.data);
  .c166.small.shuge.bss 0x4e0800 (NOLOAD) : {
    _c166_shuge_bss_start = .;
    *(.c166.small.shuge.bss .c166.small.shuge.bss.*)
    _c166_shuge_bss_end = .;
  }
  ASSERT(SIZEOF(.c166.small.far.data) <= 0x800,
         "far initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.small.far.bss) <= 0x800,
         "far zero-initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.small.huge.data) <= 0x800,
         "huge initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.small.huge.bss) <= 0x800,
         "huge zero-initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.small.shuge.data) <= 0x800,
         "shuge initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.small.shuge.bss) <= 0x800,
         "shuge zero-initialized data exceeds its region")
  /* F600..F61F is the startup register bank selected by startup.s. */
  .c166.regbank 0xf620 (NOLOAD) : { *(.c166.regbank) }
  ASSERT(SIZEOF(.c166.regbank) <= 0xe0,
         "register banks exceed reserved IRAM")
}
