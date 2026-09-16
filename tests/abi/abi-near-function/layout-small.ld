SECTIONS {
  .c166.near.entry 0x18000 : AT(0x18000) { *(.c166.near.entry) }
  .c166.near.driver 0x19000 : AT(0x19000) { *(.c166.near.driver) }
  .c166.near.text 0x1a000 : AT(0x1a000) {
    *(.c166.near.text .c166.near.text.*)
  }
  .c166.near.base 0x1c000 : AT(0x1c000) { *(.c166.near.base) }
} INSERT AFTER .c166.regbank;
