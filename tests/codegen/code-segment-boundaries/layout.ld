SECTIONS {
  .c166.near.text : {
    *(.c166.near.text .c166.near.text.*)
  }
} INSERT AFTER .text;

SECTIONS {
  .wrap.call.forward 0x190000 : AT(0x190000) {
    _boundary_wrap_call_forward = .;
    *(.wrap.call.forward)
  }
  .edge.call.forward 0x19fff8 : AT(0x19fff8) {
    _boundary_edge_call_forward = .;
    *(.edge.call.forward)
  }
  .edge.call.backward 0x1a0000 : AT(0x1a0000) {
    _boundary_edge_call_backward = .;
    *(.edge.call.backward)
  }
  .wrap.call.backward 0x1afff8 : AT(0x1afff8) {
    _boundary_wrap_call_backward = .;
    *(.wrap.call.backward)
  }
  .wrap.jump.forward 0x1b0000 : AT(0x1b0000) {
    _boundary_wrap_jump_forward = .;
    *(.wrap.jump.forward)
  }
  .edge.jump.forward 0x1bfffe : AT(0x1bfffe) {
    _boundary_edge_jump_forward = .;
    *(.edge.jump.forward)
  }
  .edge.jump.backward 0x1c0000 : AT(0x1c0000) {
    _boundary_edge_jump_backward = .;
    *(.edge.jump.backward)
  }
  .wrap.jump.backward 0x1cfff8 : AT(0x1cfff8) {
    _boundary_wrap_jump_backward = .;
    *(.wrap.jump.backward)
  }
  .wrap.bit.forward 0x1d0000 : AT(0x1d0000) {
    _boundary_wrap_bit_forward = .;
    *(.wrap.bit.forward)
  }
  .edge.bit.forward 0x1dfffc : AT(0x1dfffc) {
    _boundary_edge_bit_forward = .;
    *(.edge.bit.forward)
  }
  .edge.bit.backward 0x1e0000 : AT(0x1e0000) {
    _boundary_edge_bit_backward = .;
    *(.edge.bit.backward)
  }
  .wrap.bit.backward 0x1efff8 : AT(0x1efff8) {
    _boundary_wrap_bit_backward = .;
    *(.wrap.bit.backward)
  }
  .edge.relax.uc 0x1ffffc : AT(0x1ffffc) {
    _boundary_edge_relax_uc = .;
    *(.edge.relax.uc)
  }
  .wrap.relax.uc 0x200000 : AT(0x200000) {
    _boundary_wrap_relax_uc = .;
    *(.wrap.relax.uc)
  }
  .fallthrough.net 0x210000 : AT(0x210000) {
    _boundary_fallthrough_net = .;
    *(.fallthrough.net)
  }
  .edge.relax.net 0x21fff8 : AT(0x21fff8) {
    _boundary_edge_relax_net = .;
    *(.edge.relax.net)
  }
  .wrap.relax.net 0x220000 : AT(0x220000) {
    _boundary_wrap_relax_net = .;
    *(.wrap.relax.net)
  }
  .fallthrough.bit 0x230000 : AT(0x230000) {
    _boundary_fallthrough_bit = .;
    *(.fallthrough.bit)
  }
  .edge.relax.bit 0x23fff8 : AT(0x23fff8) {
    _boundary_edge_relax_bit = .;
    *(.edge.relax.bit)
  }
  .wrap.relax.bit 0x240000 : AT(0x240000) {
    _boundary_wrap_relax_bit = .;
    *(.wrap.relax.bit)
  }
} INSERT AFTER .c166.regbank;
