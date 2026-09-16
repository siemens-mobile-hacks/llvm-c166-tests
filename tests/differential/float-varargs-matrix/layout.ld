SECTIONS {
  .float_varargs1 0x100100 : { *(.float_varargs1) }
  .float_varargs2 0x100500 : { *(.float_varargs2) }
  .float_varargs3 0x100900 : { *(.float_varargs3) }
  .float_varargs4 0x100d00 : { *(.float_varargs4) }
  .float_varargs5 0x101100 : { *(.float_varargs5) }
  .float_varargs_raw 0x101800 : { *(.float_varargs_raw) }
  .float_varargs_raw_indirect 0x102000 : { *(.float_varargs_raw_indirect) }
  .float_varargs_promote 0x102800 : { *(.float_varargs_promote) }
  .float_varargs_promote_indirect 0x103000 : {
    *(.float_varargs_promote_indirect)
  }
} INSERT AFTER .text;
