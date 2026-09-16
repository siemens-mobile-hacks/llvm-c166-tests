SECTIONS {
  .stack_page_limit 0x100100 : AT(0x100100) {
    *(.stack_page_limit)
  }
  .stack_edge_apply 0x100b00 : AT(0x100b00) {
    *(.stack_edge_apply)
  }
  .stack_escape_store 0x100d00 : AT(0x100d00) {
    *(.stack_escape_store)
  }
} INSERT AFTER .text;
