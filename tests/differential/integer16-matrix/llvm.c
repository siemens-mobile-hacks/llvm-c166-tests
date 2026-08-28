#include "operations.h"

DEFINE_INTEGER16_EVAL(
    llvm_integer16_eval,
    __attribute__((noinline, section(".llvm_integer16_eval"))))
