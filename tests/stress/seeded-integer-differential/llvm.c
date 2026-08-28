#include "operations.h"

DEFINE_SEEDED_INTEGER_EVAL(
    llvm_seeded_integer_eval,
    __attribute__((noinline, section(".llvm_seeded_integer_eval"))))
