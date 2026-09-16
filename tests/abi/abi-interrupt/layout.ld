SECTIONS {
  .c166.vector.124 0x0001f0 : AT(0x0001f0) {
    KEEP(*(.c166.vector.124))
  }
} INSERT AFTER .reset;

SECTIONS {
  .c166.interrupt.text 0x190000 : AT(0x190000) {
    *(.c166.interrupt.text)
  }
  .c166.interrupt.data 0x410000 : AT(0x410000) {
    *(.c166.interrupt.data)
  }
} INSERT BEFORE .rodata;
