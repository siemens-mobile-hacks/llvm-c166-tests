SECTIONS {
  .c166.vector.35 0x00008c : AT(0x00008c) {
    KEEP(*(.c166.vector.35))
  }
  .c166.vector.123 0x0001ec : AT(0x0001ec) {
    KEEP(*(.c166.vector.123))
  }
  .c166.vector.124 0x0001f0 : AT(0x0001f0) {
    KEEP(*(.c166.vector.124))
  }
  .c166.near.text 0xc100 : AT(0xc100) {
    *(.c166.near.text .c166.near.text.*)
  }
  .c166.interrupt.text 0xd100 : AT(0xd100) {
    *(.c166.interrupt.text)
  }
  .c166.interrupt.nested.text 0xe800 : AT(0xe800) {
    *(.c166.interrupt.nested.text)
  }
  .c166.interrupt.data 0x410000 : AT(0x410000) {
    *(.c166.interrupt.data)
  }
} INSERT BEFORE .rodata;
