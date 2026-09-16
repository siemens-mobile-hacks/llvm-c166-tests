SECTIONS {
  .global.function-pointer.target 0x100100 : AT(0x100100) {
    *(.global.function-pointer.target)
  }
  .global.function-pointer.exchange 0x100180 : AT(0x100180) {
    *(.global.function-pointer.exchange)
  }
  .global.function-pointer.call 0x100200 : AT(0x100200) {
    *(.global.function-pointer.call)
  }
  .global.function-pointer.roundtrip 0x100280 : AT(0x100280) {
    *(.global.function-pointer.roundtrip)
  }
  .global.function-pointer.get-target 0x100480 : AT(0x100480) {
    *(.global.function-pointer.get-target)
  }
  .global.function-pointer.slot 0x9000 (NOLOAD) : {
    *(.global.function-pointer.slot)
  }
} INSERT AFTER .c166.regbank;
