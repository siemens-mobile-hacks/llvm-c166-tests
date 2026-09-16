SECTIONS {
  .global.lifecycle.text 0xd000 : AT(0xd000) {
    *(.global.lifecycle.text)
  }
} INSERT AFTER .c166.regbank;
