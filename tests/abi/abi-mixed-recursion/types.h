#ifndef ABI_MIXED_RECURSION_TYPES_H
#define ABI_MIXED_RECURSION_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_mixed_pair {
  abi_u16 first;
  abi_u16 second;
};

typedef abi_u32 (*abi_mixed_fn)(abi_u16, abi_u16, abi_u16 *, float,
                                struct abi_mixed_pair, abi_u16);

typedef char abi_mixed_pair_size_must_be_4[
    sizeof(struct abi_mixed_pair) == 4 ? 1 : -1];

extern float mixed_observed_float;
extern struct abi_mixed_pair mixed_observed_pair;
extern abi_u16 mixed_observed_pointed;
extern abi_u16 mixed_observed_tail;

void mixed_observe(float value, struct abi_mixed_pair pair,
                   abi_u16 pointed, abi_u16 tail);
abi_u32 mixed_recursion_a(abi_u16 depth, abi_u16 word, abi_u16 *pointer,
                          float value, struct abi_mixed_pair pair,
                          abi_u16 tail);
abi_u32 mixed_recursion_b(abi_u16 depth, abi_u16 word, abi_u16 *pointer,
                          float value, struct abi_mixed_pair pair,
                          abi_u16 tail);

#endif
