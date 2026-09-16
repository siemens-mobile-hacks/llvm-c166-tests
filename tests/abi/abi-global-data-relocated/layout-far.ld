SECTIONS {
  .global.lifecycle.text 0x190000 : AT(0x190000) {
    *(.global.lifecycle.text)
  }
} INSERT AFTER .c166.regbank;
