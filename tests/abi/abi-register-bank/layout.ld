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
} INSERT AFTER .reset;

SECTIONS {
  .c166.interrupt.text 0x190000 : AT(0x190000) {
    *(.c166.interrupt.text)
  }
  .c166.interrupt.nested.text 0x191000 : AT(0x191000) {
    *(.c166.interrupt.nested.text)
  }
  .c166.interrupt.data 0x410000 : AT(0x410000) {
    *(.c166.interrupt.data)
  }
} INSERT AFTER .c166.regbank;
