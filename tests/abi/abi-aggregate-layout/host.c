#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern abi_u16 llvm_entry_proxy(abi_u16 seed, abi_u16 *components);
extern abi_u16 hash_tasking_layout(abi_u16 seed, abi_u16 *components);
extern abi_u16 hash_llvm_layout(abi_u16 seed, abi_u16 *components);

static abi_u16 tasking_values[10];
static abi_u16 llvm_values[10];
static abi_u16 entry_values[10];

struct layout_vector {
  abi_u16 seed;
  abi_u16 golden[10];
};

#define LAYOUT_VECTOR(id, seed, chars, padded, nested, overlay, packed, bits, \
                      packed_bits, bit_edges, nested_packed, hash) \
  { seed, { chars, padded, nested, overlay, packed, bits, packed_bits, \
            bit_edges, nested_packed, hash } },

static const struct layout_vector layout_vectors[] = {
  ABI_AGGREGATE_LAYOUT_VECTORS(LAYOUT_VECTOR)
};

static void run_layout_vector(abi_u16 vector_id,
                              const struct layout_vector *vector) {
  abi_u16 base = (vector_id - 1U) * 30U;
  abi_u16 selection;
  tasking_values[9] = hash_tasking_layout(vector->seed, tasking_values);
  llvm_values[9] = hash_llvm_layout(vector->seed, llvm_values);
  entry_values[9] = llvm_entry_proxy(vector->seed, entry_values);
  for (selection = 0; selection != 10U; ++selection) {
    abi_u16 golden = vector->golden[selection];
    abi_u16 selection_base = base + selection * 3U;
    c166_test_check_u32(selection_base + 1U, golden,
                        tasking_values[selection]);
    c166_test_check_u32(selection_base + 2U, golden,
                        llvm_values[selection]);
    c166_test_check_u32(selection_base + 3U, golden,
                        entry_values[selection]);
  }
}

void main(void) {
  abi_u16 index;
  c166_test_begin(267, 0x1660010bUL);
  for (index = 0; index != sizeof(layout_vectors) / sizeof(layout_vectors[0]);
       ++index)
    run_layout_vector(index + 1U, &layout_vectors[index]);
  c166_test_finish();
  simulator_stop();
}

