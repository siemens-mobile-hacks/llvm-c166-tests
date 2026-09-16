SECTIONS {
  .abi_varargs_stream 0x4000 : { *(.abi_varargs_stream) }
  .abi_varargs1 0x5000 : { *(.abi_varargs1) }
  .abi_varargs2 0x5400 : { *(.abi_varargs2) }
  .abi_varargs3 0x5800 : { *(.abi_varargs3) }
  .abi_varargs4 0x5c00 : { *(.abi_varargs4) }
  .abi_varargs5 0x6000 : { *(.abi_varargs5) }
  .abi_varargs_dispatch 0x6400 : { *(.abi_varargs_dispatch) }
  .abi_varargs_stream_call 0x6800 : { *(.abi_varargs_stream_call) }
} INSERT AFTER .text;
