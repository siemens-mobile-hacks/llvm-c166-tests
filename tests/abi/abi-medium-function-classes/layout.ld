SECTIONS {
  .c166.near.text 0x2000 : AT(0x2000) {
    *(.c166.near.text .c166.near.text.*)
  }
  .medium.near.callback 0x8000 : AT(0x8000) {
    *(.medium.near.callback)
  }
  .medium.near.dispatch 0xc100 : AT(0xc100) {
    *(.medium.near.dispatch)
  }
  .medium.near.target 0xc900 : AT(0xc900) { *(.medium.near.target) }
  .c166.icall 0xef00 : AT(0xef00) { *(.c166.icall) }
  .medium.huge.callback 0xf0000 : AT(0xf0000) {
    *(.medium.huge.callback)
  }
  .medium.huge.target 0x180100 : AT(0x180100) {
    *(.medium.huge.target)
  }
} INSERT BEFORE .rodata;
