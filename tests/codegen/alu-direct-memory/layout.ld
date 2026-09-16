SECTIONS {
  .c166.near.text : {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT AFTER .text;

SECTIONS {
  .alu.operand 0x420000 : AT(0x420000) {
    _alu_operand = .;
    *(.alu.operand)
  }
} INSERT AFTER .c166.regbank;
