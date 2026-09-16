SECTIONS {
  .abi.recursion.alternate-leaf 0x8100 : AT(0x8100) {
    *(.abi.recursion.alternate-leaf)
  }
  .abi.recursion.alternate 0x8300 : AT(0x8300) {
    *(.abi.recursion.alternate)
  }
  .abi.recursion.alternate-primary-leaf 0x8d00 : AT(0x8d00) {
    *(.abi.recursion.alternate-primary-leaf)
  }
  .abi.recursion.primary-leaf 0xc100 : AT(0xc100) {
    *(.abi.recursion.primary-leaf)
  }
  .abi.recursion.primary 0xc300 : AT(0xc300) {
    *(.abi.recursion.primary)
  }
  .abi.recursion.primary-alternate-leaf 0xcd00 : AT(0xcd00) {
    *(.abi.recursion.primary-alternate-leaf)
  }
  .abi.recursion.forward-alternate 0xd700 : AT(0xd700) {
    *(.abi.recursion.forward-alternate)
  }
  .c166.near.text 0xd800 : AT(0xd800) {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT BEFORE .rodata;
