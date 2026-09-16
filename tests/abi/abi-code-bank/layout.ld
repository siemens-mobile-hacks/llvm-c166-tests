SECTIONS {
  .c166.banksw 0x17fe00 : AT(0x17fe00) { *(.c166.banksw) }
  .c166.icall 0x17ff00 : AT(0x17ff00) { *(.c166.icall) }
} INSERT BEFORE .rodata;

SECTIONS {
  .c166.banksw.data 0x4f0000 : AT(0x4f0000) {
    *(.c166.banksw.data)
  }
} INSERT AFTER .c166.regbank;
