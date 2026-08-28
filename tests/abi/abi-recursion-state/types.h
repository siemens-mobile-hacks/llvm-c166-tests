#ifndef ABI_RECURSION_STATE_TYPES_H
#define ABI_RECURSION_STATE_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

struct abi_recursion_frame {
  abi_u32 before;
  abi_u16 words[8];
  abi_u32 after;
};

typedef char abi_recursion_frame_size_must_be_24[
    sizeof(struct abi_recursion_frame) == 24 ? 1 : -1];

abi_u16 tasking_recursion_leaf(abi_u16 depth, abi_u16 seed);
abi_u16 llvm_recursion_leaf_bridge(abi_u16 depth, abi_u16 seed);
abi_u32 tasking_recursive(abi_u16 depth, abi_u16 seed);
abi_u32 tasking_recursive_llvm_leaf(abi_u16 depth, abi_u16 seed);

#endif
