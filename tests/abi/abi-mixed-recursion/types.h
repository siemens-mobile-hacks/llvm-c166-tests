#ifndef ABI_MIXED_RECURSION_TYPES_H
#define ABI_MIXED_RECURSION_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_mixed_pair {
  abi_u16 first;
  abi_u16 second;
};

typedef char abi_mixed_pair_size_must_be_4[
    sizeof(struct abi_mixed_pair) == 4 ? 1 : -1];

extern float tasking_mixed_float_values[];
extern float tasking_mixed_observed_float;
extern struct abi_mixed_pair tasking_mixed_observed_pair;
extern abi_u16 tasking_mixed_observed_pointed;
extern abi_u16 tasking_mixed_observed_tail;

void tasking_mixed_observe(float value, struct abi_mixed_pair pair,
                           abi_u16 pointed, abi_u16 tail);
abi_u32 tasking_mixed_recursion(abi_u16 depth, abi_u16 word,
                                abi_u16 *pointer, float value,
                                struct abi_mixed_pair pair, abi_u16 tail);
abi_u32 llvm_mixed_bridge(abi_u16 depth, abi_u16 word, abi_u16 *pointer,
                          float value, struct abi_mixed_pair pair,
                          abi_u16 tail);

#endif
