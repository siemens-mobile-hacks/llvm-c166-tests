#include <stdint.h>
#include <stdio.h>

#define CONTROL_FLOW_MATRIX_TYPES_H
typedef uint16_t abi_u16;
typedef int16_t abi_s16;
typedef uint32_t abi_u32;
typedef int32_t abi_s32;

#include "operations.h"
#include "vectors.inc"

DEFINE_CONTROL_FLOW_EVAL(native_control_flow_eval, static)

struct vector {
  abi_u16 id;
  abi_u32 a;
  abi_u32 b;
  abi_u16 count;
};

#define VECTOR(id, a, b, count) {(id), (a), (b), (count)},
static const struct vector vectors[] = {ABI_CONTROL_FLOW_VECTORS(VECTOR)};
#undef VECTOR

static abi_u32 signature_step(abi_u32 signature, abi_u16 case_id,
                              abi_u32 actual) {
  abi_u32 tag = ((abi_u32)case_id << 16) | case_id;
  return ((signature << 5) | (signature >> 27)) ^ actual ^ tag;
}

int main(void) {
  abi_u32 signature = 0x1660011bUL;
  abi_u32 reference_signature;
  unsigned int vector_index;
  abi_u16 operation;

  for (vector_index = 0; vector_index < sizeof(vectors) / sizeof(vectors[0]);
       ++vector_index) {
    const struct vector *vector = &vectors[vector_index];
    for (operation = 0; operation < ABI_CONTROL_FLOW_OP_COUNT; ++operation) {
      abi_u16 case_id = (abi_u16)(vector_index * ABI_CONTROL_FLOW_OP_COUNT +
                                  operation + 1U);
      signature = signature_step(
          signature, case_id,
          native_control_flow_eval(operation, vector->a, vector->b,
                                   vector->count));
    }
  }
  reference_signature = signature;
  signature = signature_step(signature, 0x1000U, reference_signature);
  for (vector_index = 0; vector_index < sizeof(vectors) / sizeof(vectors[0]);
       ++vector_index) {
    const struct vector *vector = &vectors[vector_index];
    for (operation = 0; operation < ABI_CONTROL_FLOW_OP_COUNT; ++operation) {
      abi_u16 case_id = (abi_u16)(0x2000U +
          vector_index * ABI_CONTROL_FLOW_OP_COUNT + operation);
      signature = signature_step(
          signature, case_id,
          native_control_flow_eval(operation, vector->a, vector->b,
                                   vector->count));
    }
  }
  printf("reference=0x%08x\nfinal=0x%08x\nvectors=%u\n",
         reference_signature, signature,
         (unsigned int)(sizeof(vectors) / sizeof(vectors[0])));
  return 0;
}
