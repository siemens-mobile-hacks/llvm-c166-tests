#include "types.h"
#include "functions.h"
#include "layout-hash.h"

#define ABI_TASKING_CALL(name) tasking_##name

__attribute__((noinline))
abi_u16 llvm_entry(abi_u16 seed, abi_u16 *components) {
  abi_u16 result;
  ABI_HASH_LAYOUT(ABI_TASKING_CALL, ABI_INIT_PACKED_BITS_FIELDS, seed,
                  components, result);
  return result;
}

DEFINE_LAYOUT_FUNCTIONS(
    llvm, __attribute__((noinline, section(".llvm_layout0"))),
    __attribute__((noinline, section(".llvm_layout1"))),
    __attribute__((noinline, section(".llvm_layout2"))),
    __attribute__((noinline, section(".llvm_layout3"))),
    __attribute__((noinline, section(".llvm_layout4"))),
    __attribute__((noinline, section(".llvm_layout5"))),
    __attribute__((noinline, section(".llvm_layout6"))))

DEFINE_LAYOUT_BIT_EDGES_FUNCTION(
    llvm, __attribute__((noinline, section(".llvm_layout7"))))

DEFINE_LAYOUT_NESTED_PACKED_FUNCTION(
    llvm, __attribute__((noinline, section(".llvm_layout8"))))
