SECTIONS {
  .seeded_f32_to_i32 0x6000 : { *(.seeded_f32_to_i32) }
  .seeded_f32_to_u32 0x6100 : { *(.seeded_f32_to_u32) }
} INSERT AFTER .text;
