SECTIONS {
  .abi.stackparm.callee 0x101300 : AT(0x101300) { *(.abi.stackparm.callee) }
  .abi.stackparm.float-callee 0x101400 : AT(0x101400) { *(.abi.stackparm.float-callee) }
  .abi.stackparm.double-callee 0x101500 : AT(0x101500) { *(.abi.stackparm.double-callee) }
  .abi.stackparm.routes 0x102000 : AT(0x102000) { *(.abi.stackparm.routes) }
} INSERT BEFORE .rodata;
