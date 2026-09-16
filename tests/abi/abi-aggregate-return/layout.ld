SECTIONS {
  .aggregate_return1 0x100100 : AT(0x100100) { *(.aggregate_return1) }
  .aggregate_return2 0x100200 : AT(0x100200) { *(.aggregate_return2) }
  .aggregate_return3 0x100300 : AT(0x100300) { *(.aggregate_return3) }
  .aggregate_return4 0x100400 : AT(0x100400) { *(.aggregate_return4) }
  .aggregate_return5 0x100500 : AT(0x100500) { *(.aggregate_return5) }
  .aggregate_return6 0x100600 : AT(0x100600) { *(.aggregate_return6) }
  .aggregate_return7 0x100700 : AT(0x100700) { *(.aggregate_return7) }
  .aggregate_return8 0x100800 : AT(0x100800) { *(.aggregate_return8) }
  .aggregate_return_tail 0x100900 : AT(0x100900) {
    *(.aggregate_return_tail)
  }
} INSERT AFTER .text;
