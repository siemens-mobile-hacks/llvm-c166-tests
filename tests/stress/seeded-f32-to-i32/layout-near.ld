SECTIONS {
  .seeded_f32_to_i32 0xc100 : { *(.seeded_f32_to_i32) }
  .seeded_f32_to_u32 0xc200 : { *(.seeded_f32_to_u32) }
} INSERT AFTER .text;
