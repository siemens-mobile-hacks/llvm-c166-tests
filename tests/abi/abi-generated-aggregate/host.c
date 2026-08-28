#include "c166-test-result.h"
#include "vectors.inc"

typedef unsigned int abi_u16;

extern void tasking_generated_run(abi_u16 seed, abi_u16 *output);
extern void llvm_entry_proxy(abi_u16 seed, abi_u16 *output);

struct generated_vector {
  abi_u16 seed;
  abi_u16 golden[GEN_AGGREGATE_MAX_SHAPE_COUNT];
};

#define GEN_VECTOR(id, seed, v01, v02, v03, v04, v05, v06, v07, v08, v09, \
                   v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                   v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, \
                   v32) \
  { seed, { v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11, v12, \
            v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, \
            v25, v26, v27, v28, v29, v30, v31, v32 } },

static const struct generated_vector generated_vectors[] = {
  GEN_AGGREGATE_VECTORS(GEN_VECTOR)
};

void simulator_stop(void) {
  for (;;)
    ;
}

static void run_vector(abi_u16 vector_index,
                       const struct generated_vector *vector) {
  abi_u16 tasking_values[GEN_AGGREGATE_SHAPE_COUNT];
  abi_u16 llvm_values[GEN_AGGREGATE_SHAPE_COUNT];
  abi_u16 shape_index;

  tasking_generated_run(vector->seed, tasking_values);
  llvm_entry_proxy(vector->seed, llvm_values);
  for (shape_index = 0; shape_index != GEN_AGGREGATE_SHAPE_COUNT;
       ++shape_index) {
    abi_u16 case_base =
        (abi_u16)(vector_index * GEN_AGGREGATE_SHAPE_COUNT * 2U +
                  shape_index * 2U);
    c166_test_set_context(vector->seed, (abi_u16)(shape_index + 1U), 0, 0, 0);
    c166_test_check_u32((abi_u16)(case_base + 1U),
                        vector->golden[shape_index],
                        tasking_values[shape_index]);
    c166_test_check_u32((abi_u16)(case_base + 2U),
                        vector->golden[shape_index],
                        llvm_values[shape_index]);
  }
}

void main(void) {
  abi_u16 vector_index;
  c166_test_begin(309, 0x16600135UL);
  for (vector_index = 0;
       vector_index != sizeof(generated_vectors) / sizeof(generated_vectors[0]);
       ++vector_index)
    run_vector(vector_index, &generated_vectors[vector_index]);
  c166_test_finish();
  simulator_stop();
}

