SECTIONS {
  .float_varargs1 0x4000 : { *(.float_varargs1) }
  .float_varargs2 0x4400 : { *(.float_varargs2) }
  .float_varargs3 0x4800 : { *(.float_varargs3) }
  .float_varargs4 0x4c00 : { *(.float_varargs4) }
  .float_varargs5 0x5000 : { *(.float_varargs5) }
  .float_varargs_raw 0x5800 : { *(.float_varargs_raw) }
  .float_varargs_raw_indirect 0x6000 : { *(.float_varargs_raw_indirect) }
  .float_varargs_promote 0x6800 : { *(.float_varargs_promote) }
  .float_varargs_promote_indirect 0x7000 : {
    *(.float_varargs_promote_indirect)
  }
} INSERT AFTER .text;
