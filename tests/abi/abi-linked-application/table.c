#include "types.h"

extern abi_u16 linked_archive_transform(abi_u16 value);
extern abi_u16 linked_selected_transform(abi_u16 value);
extern abi_u16 linked_common_update(abi_u16 value);

__attribute__((section(".linked.rodata")))
const linked_operation linked_operations[3] = {
    linked_archive_transform,
    linked_selected_transform,
    linked_common_update,
};

__attribute__((section(".linked.data")))
abi_u16 linked_marker = LINKED_MARKER;
