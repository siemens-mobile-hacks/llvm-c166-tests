SECTIONS {
  .abi_varargs_aggregate 0x6000 : { *(.abi_varargs_aggregate) }
  .abi_varargs_aggregate_repeat 0x6400 :
      { *(.abi_varargs_aggregate_repeat) }
} INSERT AFTER .text;
