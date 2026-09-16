ENTRY(_reset)

SECTIONS {
  .reset 0 : { KEEP(*(.reset)) }
  .c166.halt 0x0fe0 : { KEEP(*(.c166.halt)) }
  .text 0x1000 : { *(.text.start) *(.text .text.*) }
  /* DPP2 maps the logical near window at 0x8000 to physical 0x10000.
     Keep ordinary ROM beyond the fixed code-test arena and below RAM. */
  .rodata 0x300000 : { *(.rodata .rodata.*) }
  .c166.xnear.data 0x6000 :
      AT(ALIGN(LOADADDR(.rodata) + SIZEOF(.rodata), 2)) {
    _c166_xnear_data_start = .;
    *(.c166.xnear.data .c166.xnear.data.*)
    _c166_xnear_data_end = .;
  }
  _c166_xnear_data_load_start = LOADADDR(.c166.xnear.data);
  .c166.xnear.bss 0x6800 (NOLOAD) : {
    _c166_xnear_bss_start = .;
    *(.c166.xnear.bss .c166.xnear.bss.*)
    _c166_xnear_bss_end = .;
  }
  .c166.near.data 0x8000 :
      AT(ALIGN(LOADADDR(.c166.xnear.data) + SIZEOF(.c166.xnear.data), 2)) {
    _c166_near_data_start = .;
    *(.c166.near.data .c166.near.data.*)
    _c166_near_data_end = .;
  }
  _c166_near_data_load_start = LOADADDR(.c166.near.data);
  .c166.near.bss 0x8800 (NOLOAD) : {
    _c166_near_bss_start = .;
    *(.c166.near.bss .c166.near.bss.*)
    _c166_near_bss_end = .;
  }
  .data 0x400000 :
      AT(ALIGN(LOADADDR(.c166.near.data) + SIZEOF(.c166.near.data), 2)) {
    _c166_data_start = .;
    *(.data .data.*)
    _c166_data_end = .;
  }
  _c166_data_load_start = LOADADDR(.data);
  .bss (NOLOAD) : {
    _c166_bss_start = .;
    *(.bss .bss.* COMMON)
    _c166_bss_end = .;
  }
  .noinit (NOLOAD) : { *(.noinit .noinit.*) }
  ASSERT(SIZEOF(.c166.xnear.data) <= 0x800,
         "xnear initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.xnear.bss) <= 0x800,
         "xnear zero-initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.near.data) <= 0x800,
         "near initialized data exceeds its region")
  ASSERT(SIZEOF(.c166.near.bss) <= 0x800,
         "near zero-initialized data exceeds its region")
  /* F600..F61F is the startup register bank selected by startup.s. */
  .c166.regbank 0xf620 (NOLOAD) : { *(.c166.regbank) }
  ASSERT(SIZEOF(.c166.regbank) <= 0xe0,
         "register banks exceed reserved IRAM")
}
