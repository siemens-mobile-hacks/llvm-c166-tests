SECTIONS {
  .c166.vector.1 0x000004 : AT(0x000004) {
    KEEP(*(.c166.vector.1))
  }
  .c166.vector.63 0x0000fc : AT(0x0000fc) {
    KEEP(*(.c166.vector.63))
  }
  .c166.vector.127 0x0001fc : AT(0x0001fc) {
    KEEP(*(.c166.vector.127))
  }
} INSERT AFTER .reset;

SECTIONS {
  .c166.near.text : {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT AFTER .text;

SECTIONS {
  .c166.interrupt.text 0x190000 : AT(0x190000) {
    *(.c166.interrupt.text)
  }
} INSERT AFTER .c166.regbank;
