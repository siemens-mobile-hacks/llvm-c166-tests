#include "c166_fp_bits.h"
#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

extern volatile float external_float_array[4];
extern volatile struct abi_float_record external_float_record_src;
extern volatile struct abi_float_record external_float_record_dst;
extern volatile float c166_float_array[4];
extern volatile struct abi_float_record c166_float_record;

extern float c166_float_array_load_external(unsigned int);
extern void c166_float_array_store_external(unsigned int, float);
extern float c166_float_array_load_own(unsigned int);
extern void c166_float_array_store_own(unsigned int, float);
extern float c166_float_record_load_external(unsigned int);
extern void c166_float_record_store_external(unsigned int, float);
extern float c166_float_record_load_own(unsigned int);
extern void c166_float_record_store_own(unsigned int, float);
extern float c166_float_record_copy_external(unsigned int);
extern float c166_float_record_local(unsigned int, float);

#define STORAGE_ROW(id, bits) bits,
static const abi_u32 storage_vectors[] = {
    ABI_FLOAT32_STORAGE_VECTORS(STORAGE_ROW)};
#undef STORAGE_ROW

static void check_float(float value, abi_u32 expected, const char *name) {
  tap_is_u32(c166_f32_to_bits(value), expected, name);
}

static void initialize_record(volatile struct abi_float_record *record,
                              abi_u32 bits) {
  record->tag = 0x1357U;
  c166_f32_store_bits(&record->first, bits);
  record->guard = 0x2468U;
  c166_f32_store_bits(&record->second, bits);
  record->tail = 0x89abcdefUL;
}

static void run_storage_vector(abi_u16 vector_id, abi_u32 bits) {
  abi_u16 index = (abi_u16)(vector_id & 3U);
  abi_u16 which = (abi_u16)(vector_id & 1U);
  volatile float *field;
  float value = c166_f32_from_bits(bits);

  c166_f32_store_bits(&external_float_array[index], bits);
  check_float(c166_float_array_load_external(index), bits,
              "external array load");
  c166_float_array_store_external(index, value);
  tap_is_u32(c166_f32_load_bits(&external_float_array[index]), bits,
             "external array store");

  c166_f32_store_bits(&c166_float_array[index], bits);
  check_float(c166_float_array_load_own(index), bits, "own array load");
  c166_float_array_store_own(index, value);
  tap_is_u32(c166_f32_load_bits(&c166_float_array[index]), bits,
             "own array store");

  field = which ? &external_float_record_src.second
                : &external_float_record_src.first;
  c166_f32_store_bits(field, bits);
  check_float(c166_float_record_load_external(which), bits,
              "external record load");
  c166_float_record_store_external(which, value);
  tap_is_u32(c166_f32_load_bits(field), bits, "external record store");

  field = which ? &c166_float_record.second : &c166_float_record.first;
  c166_f32_store_bits(field, bits);
  check_float(c166_float_record_load_own(which), bits, "own record load");
  c166_float_record_store_own(which, value);
  tap_is_u32(c166_f32_load_bits(field), bits, "own record store");

  initialize_record(&external_float_record_src, bits);
  check_float(c166_float_record_copy_external(0U), bits,
              "external record copy first");
  check_float(c166_float_record_copy_external(1U), bits,
              "external record copy second");
  check_float(c166_float_record_local(0U, value), bits,
              "local record copy first");
  check_float(c166_float_record_local(1U, value), bits,
              "local record copy second");
}

void main(void) {
  static const abi_u32 initial[] = {0x3f800000UL, 0xc0200000UL, 0x00000001UL,
                                    0x80000000UL};
  abi_u16 index;

  tap_plan(6U + ABI_FLOAT32_STORAGE_VECTOR_COUNT * 12U);
  for (index = 0U; index != 4U; ++index)
    check_float(c166_float_array_load_own(index), initial[index],
                "array initializer");
  check_float(c166_float_record_load_own(0U), initial[0],
              "record first initializer");
  check_float(c166_float_record_load_own(1U), initial[1],
              "record second initializer");
  for (index = 0U; index != ABI_FLOAT32_STORAGE_VECTOR_COUNT; ++index)
    run_storage_vector(index, storage_vectors[index]);
}
