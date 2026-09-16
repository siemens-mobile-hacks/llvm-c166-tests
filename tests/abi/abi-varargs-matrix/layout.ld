SECTIONS {
  .abi_varargs1 0x100100 : { *(.abi_varargs1) }
  .abi_varargs2 0x100500 : { *(.abi_varargs2) }
  .abi_varargs3 0x100900 : { *(.abi_varargs3) }
  .abi_varargs4 0x100d00 : { *(.abi_varargs4) }
  .abi_varargs5 0x101100 : { *(.abi_varargs5) }
  .abi_varargs_dispatch 0x101a00 : { *(.abi_varargs_dispatch) }
  .abi_varargs_stream 0x102000 : { *(.abi_varargs_stream) }
  .abi_varargs_stream_call 0x103000 : { *(.abi_varargs_stream_call) }
} INSERT AFTER .text;
