SECTIONS {
  .abi.recursion.alternate-leaf 0x098000 : AT(0x098000) {
    *(.abi.recursion.alternate-leaf)
  }
  .abi.recursion.alternate 0x099000 : AT(0x099000) {
    *(.abi.recursion.alternate)
  }
  .abi.recursion.alternate-primary-leaf 0x09a000 : AT(0x09a000) {
    *(.abi.recursion.alternate-primary-leaf)
  }
  .abi.recursion.primary-leaf 0x100100 : AT(0x100100) {
    *(.abi.recursion.primary-leaf)
  }
  .abi.recursion.primary 0x102000 : AT(0x102000) {
    *(.abi.recursion.primary)
  }
  .abi.recursion.primary-alternate-leaf 0x104000 : AT(0x104000) {
    *(.abi.recursion.primary-alternate-leaf)
  }
  .abi.recursion.forward-alternate 0x106000 : AT(0x106000) {
    *(.abi.recursion.forward-alternate)
  }
} INSERT BEFORE .rodata;
