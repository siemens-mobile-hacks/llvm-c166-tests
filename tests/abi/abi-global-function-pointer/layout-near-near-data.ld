SECTIONS {
  .global.function-pointer.target 0xc100 : AT(0xc100) {
    *(.global.function-pointer.target)
  }
  .global.function-pointer.exchange 0xc180 : AT(0xc180) {
    *(.global.function-pointer.exchange)
  }
  .global.function-pointer.call 0xc200 : AT(0xc200) {
    *(.global.function-pointer.call)
  }
  .global.function-pointer.roundtrip 0xc280 : AT(0xc280) {
    *(.global.function-pointer.roundtrip)
  }
  .global.function-pointer.get-target 0xc480 : AT(0xc480) {
    *(.global.function-pointer.get-target)
  }
  .global.function-pointer.slot 0x9000 (NOLOAD) : {
    *(.global.function-pointer.slot)
  }
} INSERT AFTER .c166.regbank;
