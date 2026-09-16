SECTIONS {
  .c166.near.text 0x9000 : AT(0x9000) { *(.c166.near.text .c166.near.text.*) }
  .abi.stackparm.callee 0xd300 : AT(0xd300) { *(.abi.stackparm.callee) }
  .abi.stackparm.float-callee 0xd400 : AT(0xd400) { *(.abi.stackparm.float-callee) }
  .abi.stackparm.double-callee 0xd500 : AT(0xd500) { *(.abi.stackparm.double-callee) }
  .abi.stackparm.routes 0xdcb6 : AT(0xdcb6) { *(.abi.stackparm.routes) }
} INSERT BEFORE .rodata;
