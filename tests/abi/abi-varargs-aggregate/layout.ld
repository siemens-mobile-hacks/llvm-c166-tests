SECTIONS {
  .abi_varargs_aggregate 0x100100 : { *(.abi_varargs_aggregate) }
  .abi_varargs_aggregate_repeat 0x100500 :
      { *(.abi_varargs_aggregate_repeat) }
} INSERT AFTER .text;
