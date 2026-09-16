SECTIONS {
  .c166.near.text : {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT AFTER .text;

SECTIONS {
  .ext.page 0x410000 : AT(0x410000) {
    _extended_page = .;
    *(.ext.page)
  }
  .ext.segment 0x414000 : AT(0x414000) {
    _extended_segment = .;
    *(.ext.segment)
  }
  .ext.default 0x420000 : AT(0x420000) {
    _extended_default = .;
    *(.ext.default)
  }
} INSERT AFTER .c166.regbank;
