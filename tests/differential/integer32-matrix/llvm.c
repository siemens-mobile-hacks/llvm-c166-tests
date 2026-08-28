#include "operations.h"

DEFINE_INTEGER32_EVAL(
    llvm_integer32_eval,
    __attribute__((noinline, section(".llvm_integer32_eval"))))

#undef INTEGER32_HELPER_CASES
