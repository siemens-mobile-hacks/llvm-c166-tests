#include "c166_test.h"
#include "types.h"
#include "vectors.inc"

struct layout_vector {
  abi_u16 seed;
  abi_u16 expected[10];
};

#define LAYOUT_VECTOR(id, seed, chars, padded, nested, overlay, packed, bits,  \
                      packed_bits, bit_edges, nested_packed, hash)             \
  {seed,                                                                       \
   {chars, padded, nested, overlay, packed, bits, packed_bits, bit_edges,      \
    nested_packed, hash}},

static const struct layout_vector vectors[] = {
    ABI_AGGREGATE_LAYOUT_VECTORS(LAYOUT_VECTOR)};

static void run_vector(const struct layout_vector *vector) {
  abi_u16 actual[10];
  abi_u16 index;

  actual[9] = c166_hash_layout(vector->seed, actual);
  for (index = 0; index != 10U; ++index)
    tap_is_u32(actual[index], vector->expected[index], "aggregate layout");
}

void main(void) {
  abi_u16 index;

  tap_plan(60U);
  for (index = 0; index != sizeof(vectors) / sizeof(vectors[0]); ++index)
    run_vector(&vectors[index]);
}
