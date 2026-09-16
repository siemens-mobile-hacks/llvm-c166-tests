SECTIONS {
  .seeded_f32_to_i32 0x100100 : { *(.seeded_f32_to_i32) }
  .seeded_f32_to_u32 0x100200 : { *(.seeded_f32_to_u32) }
} INSERT AFTER .text;
