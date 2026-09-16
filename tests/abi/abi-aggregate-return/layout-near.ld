SECTIONS {
  .aggregate_return1 0xc100 : AT(0xc100) { *(.aggregate_return1) }
  .aggregate_return2 0xc200 : AT(0xc200) { *(.aggregate_return2) }
  .aggregate_return3 0xc300 : AT(0xc300) { *(.aggregate_return3) }
  .aggregate_return4 0xc400 : AT(0xc400) { *(.aggregate_return4) }
  .aggregate_return5 0xc500 : AT(0xc500) { *(.aggregate_return5) }
  .aggregate_return6 0xc600 : AT(0xc600) { *(.aggregate_return6) }
  .aggregate_return7 0xc700 : AT(0xc700) { *(.aggregate_return7) }
  .aggregate_return8 0xc800 : AT(0xc800) { *(.aggregate_return8) }
  .aggregate_return_tail 0xc900 : AT(0xc900) { *(.aggregate_return_tail) }
} INSERT AFTER .c166.regbank;
