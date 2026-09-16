SECTIONS {
  .llvm_tuple_anchor 0x100100 : AT(0x100100) { *(.llvm_tuple_anchor) }
  .tuple.text 0x180000 : AT(0x180000) { *(.tuple.text) }
} INSERT AFTER .c166.regbank;
