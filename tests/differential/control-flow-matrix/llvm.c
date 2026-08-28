#define CONTROL_FLOW_LLVM 1
#include "operations.h"

#if TASKING_MODEL_IS_MEDIUM
#define LLVM_CONTROL_FLOW_SECTION ".c166.near.text"
#else
#define LLVM_CONTROL_FLOW_SECTION ".llvm_control_flow_eval"
#endif

DEFINE_CONTROL_FLOW_EVAL(
    llvm_control_flow_eval,
    __attribute__((noinline, section(LLVM_CONTROL_FLOW_SECTION))))
