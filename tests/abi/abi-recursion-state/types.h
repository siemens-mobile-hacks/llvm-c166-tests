#ifndef ABI_RECURSION_STATE_TYPES_H
#define ABI_RECURSION_STATE_TYPES_H

#include "c166_test.h"

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_recursion_frame {
  abi_u32 before;
  abi_u16 words[8];
  abi_u32 after;
};

typedef char abi_recursion_frame_size_must_be_24[
    sizeof(struct abi_recursion_frame) == 24 ? 1 : -1];

abi_u16 recursion_primary_leaf(abi_u16 depth, abi_u16 seed);
abi_u16 recursion_alternate_leaf(abi_u16 depth, abi_u16 seed);

abi_u32 recursion_primary(abi_u16 depth, abi_u16 seed);
abi_u32 recursion_primary_alternate_leaf(abi_u16 depth, abi_u16 seed);
abi_u32 recursion_alternate(abi_u16 depth, abi_u16 seed);
abi_u32 recursion_alternate_primary_leaf(abi_u16 depth, abi_u16 seed);
abi_u32 recursion_forward_alternate(abi_u16 depth, abi_u16 seed);

#endif
