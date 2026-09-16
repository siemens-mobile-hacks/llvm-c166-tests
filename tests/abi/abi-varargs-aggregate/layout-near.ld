SECTIONS {
  .abi_varargs_aggregate 0xc100 : { *(.abi_varargs_aggregate) }
  .abi_varargs_aggregate_repeat 0xc500 :
      { *(.abi_varargs_aggregate_repeat) }
} INSERT AFTER .text;
