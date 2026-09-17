cmake_minimum_required(VERSION 3.20)

project(c166_tests LANGUAGES CXX)

enable_testing()

set(C166_TEST_JOBS 8 CACHE STRING "Parallel workers for test builds")
if(NOT C166_TEST_JOBS MATCHES "^[1-9][0-9]*$")
  message(FATAL_ERROR "C166_TEST_JOBS must be a positive integer")
endif()

set(C166_TEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(tools/runner)
include(cmake/C166ProgramTest.cmake)
include(cmake/C166SizeComparison.cmake)

c166_add_test(
  NAME runtime.crt
  SOURCES tests/runtime/crt/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O2
)

c166_add_test(
  NAME runtime.bool-semantics
  SOURCES tests/runtime/bool-semantics/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
)

c166_add_test(
  NAME runtime.clz32-matrix
  SOURCES tests/runtime/clz32-matrix/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/clz32-matrix/expected.nm
  LLVM_DIS_CHECK tests/runtime/clz32-matrix/expected.dis
)

c166_add_test(
  NAME differential.integer16-matrix
  SOURCES tests/differential/integer16-matrix/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 351
  SEED 375390489
  EXPECTED_SIGNATURE 3245606799
  LLVM_NM_CHECK tests/differential/integer16-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/integer16-matrix/expected.dis
)

c166_add_test(
  NAME differential.integer32-matrix
  SOURCES tests/differential/integer32-matrix/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 510
  SEED 375390490
  EXPECTED_SIGNATURE 3818825980
  LLVM_NM_CHECK tests/differential/integer32-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/integer32-matrix/expected.dis
)

c166_add_test(
  NAME differential.control-flow-matrix
  SOURCES
    tests/differential/control-flow-matrix/test.c
    tests/differential/control-flow-matrix/padding.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 408
  SEED 375390491
  EXPECTED_SIGNATURE 4063372237
  LLVM_NM_CHECK tests/differential/control-flow-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/control-flow-matrix/expected.dis
)

c166_add_test(
  NAME differential.byte-arithmetic
  SOURCES
    tests/differential/byte-arithmetic/test.c
    size/benchmarks/byte-arithmetic.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 22
  SEED 324478056
  EXPECTED_SIGNATURE 1686064005
)

c166_add_test(
  NAME runtime.i64-comparison-matrix
  SOURCES tests/runtime/i64-comparison-matrix/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/i64-comparison-matrix/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.i64-arithmetic-matrix
  SOURCES tests/runtime/i64-arithmetic-matrix/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/i64-arithmetic-matrix/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.i64-conversion-matrix
  SOURCES tests/runtime/i64-conversion-matrix/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/i64-conversion-matrix/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.runtime
  SOURCES tests/runtime/runtime/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O2
  LLVM_NM_CHECK tests/runtime/runtime/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.backend-operation-matrix
  SOURCES tests/runtime/backend-operation-matrix/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/backend-operation-matrix/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.tasking-runtime-variants
  SOURCES tests/runtime/tasking-runtime-variants/test.c
  MODELS large small
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_NM_CHECK tests/runtime/tasking-runtime-variants/expected.nm
)

c166_add_test(
  NAME runtime.float-helper-abi
  SOURCES
    tests/runtime/float-helper-abi/test.c
    tests/runtime/float-helper-abi/probe.S
  MODELS large medium small tiny huge
  OPTIMIZATIONS O2
  LLVM_NM_CHECK tests/runtime/float-helper-abi/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    tiny=tests/runtime/float-helper-abi/expected.nm.tiny
  LLVM_DIS_CHECK tests/runtime/float-helper-abi/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    tiny=tests/runtime/float-helper-abi/expected.dis.tiny
  LLVM_ONLY
)

c166_add_test(
  NAME codegen.computed-goto
  SOURCES tests/codegen/computed-goto/llvm.c
  MODELS large medium
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/codegen/computed-goto/expected.nm
  LLVM_DIS_CHECK tests/codegen/computed-goto/expected.dis
  LLVM_ONLY
)

c166_add_test(
  NAME codegen.sfr-bitfields
  SOURCES tests/codegen/sfr-bitfields/llvm.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/codegen/sfr-bitfields/expected.nm
  LLVM_DIS_CHECK tests/codegen/sfr-bitfields/expected.dis
  LLVM_ONLY
)

c166_add_test(
  NAME runtime.atomic-runtime
  SOURCES tests/runtime/atomic-runtime/llvm.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/runtime/atomic-runtime/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME codegen.dynamic-alloca
  SOURCES
    tests/codegen/dynamic-alloca/alloca.c
    tests/codegen/dynamic-alloca/callback.c
    tests/codegen/dynamic-alloca/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/codegen/dynamic-alloca/expected.nm
  LLVM_ONLY
)

c166_add_test(
  NAME codegen.aggregate
  SOURCES tests/codegen/aggregate/test.c
  MODELS large medium small huge
  OPTIMIZATIONS O2
  LLVM_NM_CHECK tests/codegen/aggregate/expected.nm
)

c166_add_test(
  NAME codegen.branch
  SOURCES tests/codegen/branch/test.c
  MODELS large medium small huge
  OPTIMIZATIONS O2
  LLVM_NM_CHECK tests/codegen/branch/expected.nm
)

c166_add_test(
  NAME differential.float-width-matrix
  SOURCES tests/differential/float-width-matrix/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/differential/float-width-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/float-width-matrix/expected.dis
  LLVM_DIS_CHECK_MODELS medium
)

c166_add_test(
  NAME abi.scalar
  SOURCES tests/abi/abi-scalar/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 8
  SEED 375390465
  EXPECTED_SIGNATURE 193417850
  LLVM_NM_CHECK tests/abi/abi-scalar/expected.nm
)

c166_add_test(
  NAME abi.pair-r12
  SOURCES tests/abi/abi-pair-r12/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 8
  SEED 375390472
  EXPECTED_SIGNATURE 4292124532
  LLVM_NM_CHECK tests/abi/abi-pair-r12/expected.nm
)

c166_add_test(
  NAME abi.regargs
  SOURCES tests/abi/regargs/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/regargs/expected.nm
)

c166_add_test(
  NAME abi.stack
  SOURCES tests/abi/stack/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/stack/expected.nm
)

c166_add_test(
  NAME abi.structret
  SOURCES tests/abi/structret/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/structret/expected.nm
)

c166_add_test(
  NAME abi.byte-return
  SOURCES tests/abi/abi-byte-return/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 5
  SEED 375390467
  EXPECTED_SIGNATURE 179937385
  LLVM_NM_CHECK tests/abi/abi-byte-return/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-byte-return/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-byte-return/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.aggregate-args
  SOURCES tests/abi/abi-aggregate-args/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 4
  SEED 375390473
  EXPECTED_SIGNATURE 3309816721
  LLVM_NM_CHECK tests/abi/abi-aggregate-args/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-aggregate-args/expected.dis.medium
  LLVM_DIS_CHECK_MODELS medium
)

c166_add_test(
  NAME abi.farptr
  SOURCES tests/abi/farptr/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/farptr/expected.nm
)

c166_add_test(
  NAME abi.far-pointer
  SOURCES tests/abi/abi-far-pointer/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-far-pointer/expected.nm
)

c166_add_test(
  NAME abi.reverse
  SOURCES
    tests/abi/reverse/test.c
    tests/abi/reverse/callee.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/reverse/expected.nm
)

c166_add_test(
  NAME abi.saved
  SOURCES
    tests/abi/saved/test.c
    tests/abi/saved/probe.S
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_NM_CHECK tests/abi/saved/expected.nm
)

c166_add_test(
  NAME abi.long
  SOURCES
    tests/abi/abi-long/test.c
    tests/abi/abi-long/eval.c
    tests/abi/abi-long/callee.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-long/expected.nm
)

c166_add_test(
  NAME abi.aggregate-layout
  SOURCES
    tests/abi/abi-aggregate-layout/test.c
    tests/abi/abi-aggregate-layout/eval.c
    tests/abi/abi-aggregate-layout/functions.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 90
  LLVM_NM_CHECK tests/abi/abi-aggregate-layout/expected.nm
)

c166_add_test(
  NAME abi.aggregate-return
  SOURCES
    tests/abi/abi-aggregate-return/test.c
    tests/abi/abi-aggregate-return/functions.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-aggregate-return/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-aggregate-return/layout-near.ld
    small=tests/abi/abi-aggregate-return/layout-near.ld
    tiny=tests/abi/abi-aggregate-return/layout-near.ld
  LLVM_NM_CHECK tests/abi/abi-aggregate-return/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-aggregate-return/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-aggregate-return/expected.dis.near
    tiny=tests/abi/abi-aggregate-return/expected.dis.near
)

c166_add_test(
  NAME abi.near-pointer
  SOURCES tests/abi/abi-near-pointer/test.c
  MODELS large medium
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-near-pointer/layout.ld
  LLVM_NM_CHECK tests/abi/abi-near-pointer/expected.nm
)

c166_add_test(
  NAME abi.qualified-globals
  SOURCES tests/abi/abi-qualified-globals/test.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-qualified-globals/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    small=tests/abi/abi-qualified-globals/expected.nm.small
)

c166_add_test(
  NAME abi.global-data-lifecycle
  SOURCES
    tests/abi/abi-global-data-lifecycle/test.c
    tests/abi/abi-global-data-lifecycle/globals.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-global-data-lifecycle/expected.nm
)

c166_add_test(
  LLVM_ONLY
  NAME abi.global-data-relocated
  SOURCES
    tests/abi/abi-global-data-lifecycle/test.c
    tests/abi/abi-global-data-lifecycle/globals.c
  DEFINITIONS C166_TEST_RELOCATED=1
  MODELS large huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINK_OPTIONS
    -Tdata=0x430000
    --section-start=.rodata=0x440000
    --section-start=.bss=0x510000
  LLVM_LINKER_SCRIPT tests/abi/abi-global-data-relocated/layout-far.ld
  LLVM_NM_CHECK
    tests/abi/abi-global-data-lifecycle/expected.nm.relocated
)

c166_add_test(
  LLVM_ONLY
  NAME abi.global-data-relocated
  SOURCES
    tests/abi/abi-global-data-lifecycle/test.c
    tests/abi/abi-global-data-lifecycle/globals.c
  DEFINITIONS C166_TEST_RELOCATED=1
  MODELS medium
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINK_OPTIONS
    -Tdata=0x430000
    --section-start=.rodata=0x440000
    --section-start=.bss=0x510000
  LLVM_LINKER_SCRIPT tests/abi/abi-global-data-relocated/layout-near.ld
  LLVM_NM_CHECK
    tests/abi/abi-global-data-lifecycle/expected.nm.relocated-medium
)

c166_add_test(
  LLVM_ONLY
  NAME abi.global-data-relocated
  SOURCES
    tests/abi/abi-global-data-lifecycle/test.c
    tests/abi/abi-global-data-lifecycle/globals.c
  DEFINITIONS C166_TEST_RELOCATED=1
  MODELS small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINK_OPTIONS
    -Tdata=0x5200
    --section-start=.rodata=0x5600
    --section-start=.bss=0x5c00
  LLVM_LINKER_SCRIPT tests/abi/abi-global-data-relocated/layout-far.ld
  LLVM_NM_CHECK
    tests/abi/abi-global-data-lifecycle/expected.nm.relocated-small
)

c166_add_test(
  NAME abi.global-function-pointer
  SOURCES
    tests/abi/abi-global-function-pointer/test.c
    tests/abi/abi-global-function-pointer/functions.c
    tests/abi/abi-global-function-pointer/alternate.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-global-function-pointer/layout-far.ld
    medium=tests/abi/abi-global-function-pointer/layout-near.ld
    small=tests/abi/abi-global-function-pointer/layout-far-near-data.ld
    tiny=tests/abi/abi-global-function-pointer/layout-near-near-data.ld
    huge=tests/abi/abi-global-function-pointer/layout-far.ld
  LLVM_NM_CHECK tests/abi/abi-global-function-pointer/expected.nm
  LLVM_NM_FORBID_BY_MODEL
    medium=tests/abi/abi-global-function-pointer/expected.nm.forbid
    tiny=tests/abi/abi-global-function-pointer/expected.nm.forbid
  LLVM_DIS_CHECK_BY_MODEL
    large=tests/abi/abi-global-function-pointer/expected.dis
    medium=tests/abi/abi-global-function-pointer/expected.dis.near
    small=tests/abi/abi-global-function-pointer/expected.dis
    tiny=tests/abi/abi-global-function-pointer/expected.dis.near
    huge=tests/abi/abi-global-function-pointer/expected.dis
)

c166_add_test(
  NAME abi.huge-pointer
  SOURCES tests/abi/abi-huge-pointer/test.c
  MODELS large medium huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-huge-pointer/expected.nm
)

c166_add_test(
  NAME abi.pointer-casts
  SOURCES tests/abi/abi-pointer-casts/test.c
  MODELS large small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-pointer-casts/expected.nm
)

c166_add_test(
  NAME abi.far-automatic-matrix
  SOURCES
    tests/abi/abi-far-automatic-matrix/test.c
    tests/abi/abi-far-automatic-matrix/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-far-automatic-matrix/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-far-automatic-matrix/expected.dis
  LLVM_DIS_CHECK_MODELS large
)

c166_add_test(
  NAME abi.far-memory-matrix
  SOURCES
    tests/abi/abi-far-memory-matrix/test.c
    tests/abi/abi-far-memory-matrix/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-far-memory-matrix/layout.ld
  LLVM_NM_CHECK tests/abi/abi-far-memory-matrix/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-far-memory-matrix/expected.dis
  LLVM_DIS_CHECK_MODELS large
)

c166_add_test(
  NAME abi.combined-matrix
  SOURCES
    tests/abi/abi-combined-matrix/driver.c
    tests/abi/abi-combined-matrix/mixed.c
    tests/abi/abi-combined-matrix/aggregate.c
    tests/abi/abi-combined-matrix/pointer.c
    tests/abi/abi-combined-matrix/runtime.c
    tests/abi/abi-combined-matrix/float.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O2 Oz
  SIMULATOR_TIMEOUT 90
  LLVM_NM_CHECK tests/abi/abi-combined-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-combined-matrix/expected.nm.near
)

c166_add_test(
  NAME abi.word-matrix
  SOURCES
    tests/abi/abi-word-matrix/test.c
    tests/abi/abi-word-matrix/functions.c
    tests/abi/abi-word-matrix/reverse.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 120
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-word-matrix/layout.ld
    medium=tests/abi/abi-word-matrix/layout-near.ld
  LLVM_NM_CHECK tests/abi/abi-word-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-word-matrix/expected.nm.near
  LLVM_DIS_CHECK_BY_MODEL
    large=tests/abi/abi-word-matrix/expected.dis
    medium=tests/abi/abi-word-matrix/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.indirect-matrix
  SOURCES
    tests/abi/abi-indirect-matrix/test.c
    tests/abi/abi-indirect-matrix/targets.c
    tests/abi/abi-indirect-matrix/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-indirect-matrix/layout.ld
    medium=tests/abi/abi-indirect-matrix/layout-near.ld
  LLVM_NM_CHECK tests/abi/abi-indirect-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    large=tests/abi/abi-indirect-matrix/expected.nm.large
    medium=tests/abi/abi-indirect-matrix/expected.nm.near
  LLVM_DIS_CHECK_BY_MODEL
    large=tests/abi/abi-indirect-matrix/expected.dis
    medium=tests/abi/abi-indirect-matrix/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.interrupt
  SOURCES
    tests/abi/abi-interrupt/test.c
    tests/abi/abi-interrupt/probe.S
    tests/abi/abi-interrupt/vector.S
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-interrupt/layout.ld
    medium=tests/abi/abi-interrupt/layout-near.ld
    small=tests/abi/abi-interrupt/layout-small.ld
    tiny=tests/abi/abi-interrupt/layout-tiny.ld
    huge=tests/abi/abi-interrupt/layout.ld
  LLVM_NM_CHECK_BY_MODEL
    large=tests/abi/abi-interrupt/expected.nm
    medium=tests/abi/abi-interrupt/expected.nm.medium
    small=tests/abi/abi-interrupt/expected.nm
    tiny=tests/abi/abi-interrupt/expected.nm.tiny
    huge=tests/abi/abi-interrupt/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-interrupt/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-interrupt/expected.dis.medium
    tiny=tests/abi/abi-interrupt/expected.dis.medium
)

c166_add_test(
  NAME abi.abi-register-bank
  LLVM_ONLY
  SOURCES
    tests/abi/abi-register-bank/llvm.c
    tests/abi/abi-register-bank/irq-window.S
    tests/abi/abi-register-bank/probe.S
    tests/abi/abi-register-bank/vector.S
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-register-bank/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-register-bank/layout-medium.ld
  LLVM_NM_CHECK tests/abi/abi-register-bank/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-register-bank/expected.nm.medium
  LLVM_DIS_CHECK tests/abi/abi-register-bank/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-register-bank/expected.dis.medium
  SIMULATOR_TIMEOUT 120
)

c166_add_test(
  NAME abi.abi-stack-page-limit
  SOURCES
    tests/abi/abi-stack-page-limit/test.c
    tests/abi/abi-stack-page-limit/stack-page.c
    tests/abi/abi-stack-page-limit/stack-escape.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-stack-page-limit/layout.ld
    huge=tests/abi/abi-stack-page-limit/layout.ld
  LLVM_NM_CHECK tests/abi/abi-stack-page-limit/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-stack-page-limit/expected.dis
  LLVM_DIS_CHECK_MODELS large huge
  SIMULATOR_TIMEOUT 120
)

c166_add_test(
  NAME abi.recursion-state
  SOURCES
    tests/abi/abi-recursion-state/test.c
    tests/abi/abi-recursion-state/leaves.c
    tests/abi/abi-recursion-state/recursive-primary.c
    tests/abi/abi-recursion-state/recursive-alternate.c
    tests/abi/abi-recursion-state/forward.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 120
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-recursion-state/layout.ld
    medium=tests/abi/abi-recursion-state/layout-near.ld
  LLVM_NM_CHECK tests/abi/abi-recursion-state/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    large=tests/abi/abi-recursion-state/expected.nm.large
    medium=tests/abi/abi-recursion-state/expected.nm.near
  LLVM_DIS_CHECK_BY_MODEL
    large=tests/abi/abi-recursion-state/expected.dis
    medium=tests/abi/abi-recursion-state/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.multi-epilog
  SOURCES
    tests/abi/abi-multi-epilog/test.c
    tests/abi/abi-multi-epilog/kernel.c
    tests/abi/abi-multi-epilog/selector.c
    tests/abi/abi-multi-epilog/probe.S
  MODELS large small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_COMPILE_OPTIONS -mllvm -stress-regalloc=4
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-multi-epilog/layout.ld
    small=tests/abi/abi-multi-epilog/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-multi-epilog/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-multi-epilog/expected.dis
)

c166_add_test(
  NAME abi.tuple-reload
  SOURCES
    tests/abi/abi-tuple-reload/test.c
    tests/abi/abi-tuple-reload/kernel.c
    tests/abi/abi-tuple-reload/selector.c
    tests/abi/abi-tuple-reload/probe.S
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_COMPILE_OPTIONS -mllvm -stress-regalloc=3
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-tuple-reload/layout.ld
    medium=tests/abi/abi-tuple-reload/layout-near.ld
    small=tests/abi/abi-tuple-reload/layout-small.ld
    huge=tests/abi/abi-tuple-reload/layout.ld
  LLVM_NM_CHECK tests/abi/abi-tuple-reload/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-tuple-reload/expected.nm.medium
  LLVM_DIS_CHECK tests/abi/abi-tuple-reload/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-tuple-reload/expected.dis.medium
)

c166_add_test(
  NAME abi.register-pressure
  SOURCES
    tests/abi/abi-register-pressure/test.c
    tests/abi/abi-register-pressure/primary.c
    tests/abi/abi-register-pressure/alternate.c
    tests/abi/abi-register-pressure/forward.c
    tests/abi/abi-register-pressure/selectors.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-register-pressure/layout.ld
    medium=tests/abi/abi-register-pressure/layout-near.ld
    small=tests/abi/abi-register-pressure/layout-small.ld
    tiny=tests/abi/abi-register-pressure/layout-tiny.ld
    huge=tests/abi/abi-register-pressure/layout.ld
  LLVM_NM_CHECK tests/abi/abi-register-pressure/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-register-pressure/expected.nm.near
    tiny=tests/abi/abi-register-pressure/expected.nm.tiny
  LLVM_DIS_CHECK tests/abi/abi-register-pressure/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-register-pressure/expected.dis.medium
    tiny=tests/abi/abi-register-pressure/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.stackparm
  SOURCES
    tests/abi/abi-stackparm/test.c
    tests/abi/abi-stackparm/callees.c
    tests/abi/abi-stackparm/calls.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-stackparm/layout.ld
    medium=tests/abi/abi-stackparm/layout-near.ld
    small=tests/abi/abi-stackparm/layout-small.ld
    huge=tests/abi/abi-stackparm/layout.ld
  LLVM_NM_CHECK tests/abi/abi-stackparm/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-stackparm/expected.nm.near
  LLVM_DIS_CHECK tests/abi/abi-stackparm/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-stackparm/expected.dis.medium
  LLVM_DIS_CHECK_MODELS large medium
)

c166_add_test(
  NAME abi.fp-calls
  SOURCES
    tests/abi/abi-fp-calls/test.c
    tests/abi/abi-fp-calls/callees.c
    tests/abi/abi-fp-calls/calls.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-fp-calls/layout.ld
    medium=tests/abi/abi-fp-calls/layout-near.ld
    small=tests/abi/abi-fp-calls/layout-small.ld
    huge=tests/abi/abi-fp-calls/layout.ld
  LLVM_NM_CHECK tests/abi/abi-fp-calls/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-fp-calls/expected.nm.near
)

c166_add_test(
  NAME abi.generated-aggregate
  SOURCES
    tests/abi/abi-generated-aggregate/test.c
    tests/abi/abi-generated-aggregate/functions.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-generated-aggregate/expected.nm
  SIMULATOR_TIMEOUT 180
)

c166_add_test(
  NAME abi.indirect-call
  SOURCES
    tests/abi/abi-indirect-call/test.c
    tests/abi/abi-indirect-call/entry.c
    tests/abi/abi-indirect-call/target.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-indirect-call/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-indirect-call/expected.nm.near
    tiny=tests/abi/abi-indirect-call/expected.nm.near
  LLVM_NM_FORBID_BY_MODEL
    tiny=tests/abi/abi-indirect-call/expected.nm.forbid
  LLVM_DIS_CHECK tests/abi/abi-indirect-call/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-indirect-call/expected.dis.near
    tiny=tests/abi/abi-indirect-call/expected.dis.near
)

c166_add_test(
  NAME abi.nested-call-chains
  SOURCES
    tests/abi/abi-nested-call-chains/test.c
    tests/abi/abi-nested-call-chains/dispatch.c
    tests/abi/abi-nested-call-chains/callbacks.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-nested-call-chains/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-nested-call-chains/expected.nm.near
    tiny=tests/abi/abi-nested-call-chains/expected.nm.near
  LLVM_NM_FORBID_BY_MODEL
    medium=tests/abi/abi-nested-call-chains/expected.nm.forbid
    tiny=tests/abi/abi-nested-call-chains/expected.nm.forbid
  LLVM_DIS_CHECK tests/abi/abi-nested-call-chains/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-nested-call-chains/expected.dis.near
    tiny=tests/abi/abi-nested-call-chains/expected.dis.near
)

c166_add_test(
  NAME abi.near-function
  SOURCES
    tests/abi/abi-near-function/test.c
    tests/abi/abi-near-function/entry.c
    tests/abi/abi-near-function/callers.c
    tests/abi/abi-near-function/targets.c
  MODELS large small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/abi/abi-near-function/layout-large.ld
    small=tests/abi/abi-near-function/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-near-function/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-near-function/expected.dis
)

c166_add_test(
  NAME abi.medium-function-classes
  SOURCES
    tests/abi/abi-medium-function-classes/test.c
    tests/abi/abi-medium-function-classes/dispatch.c
    tests/abi/abi-medium-function-classes/callbacks.c
    tests/abi/abi-medium-function-classes/targets.c
  MODELS medium
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 180
  LLVM_LINKER_SCRIPT tests/abi/abi-medium-function-classes/layout.ld
  LLVM_NM_CHECK tests/abi/abi-medium-function-classes/expected.nm.medium
  LLVM_DIS_CHECK tests/abi/abi-medium-function-classes/expected.dis.medium
)

c166_add_test(
  NAME codegen.bit-binary
  SOURCES
    tests/codegen/bit-binary/test.c
    tests/codegen/bit-binary/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  SIMULATOR_TIMEOUT 90
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/bit-binary/expected.nm
)

c166_add_test(
  NAME codegen.bit-branch
  SOURCES
    tests/codegen/bit-branch/test.c
    tests/codegen/bit-branch/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/bit-branch/expected.nm
)

c166_add_test(
  NAME codegen.bit-writeback
  SOURCES
    tests/codegen/bit-writeback/test.c
    tests/codegen/bit-writeback/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/bit-writeback/expected.nm
)

c166_add_test(
  NAME codegen.bit-update
  SOURCES
    tests/codegen/bit-update/test.c
    tests/codegen/bit-update/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/bit-update/expected.nm
)

c166_add_test(
  NAME codegen.bit-field
  SOURCES
    tests/codegen/bit-field/test.c
    tests/codegen/bit-field/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/bit-field/expected.nm
)

c166_add_test(
  NAME codegen.mov-extend
  SOURCES
    tests/codegen/mov-extend/test.c
    tests/codegen/mov-extend/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/mov-buffer.ld
  LLVM_NM_CHECK tests/codegen/mov-extend/expected.nm
)

c166_add_test(
  NAME codegen.mov-memory
  SOURCES
    tests/codegen/mov-memory/test.c
    tests/codegen/mov-memory/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/mov-buffer.ld
  LLVM_NM_CHECK tests/codegen/mov-memory/expected.nm
)

c166_add_test(
  NAME codegen.mov-indirect
  SOURCES
    tests/codegen/mov-indirect/test.c
    tests/codegen/mov-indirect/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/mov-buffer.ld
  LLVM_NM_CHECK tests/codegen/mov-indirect/expected.nm
)

c166_add_test(
  NAME codegen.shift-rotate
  SOURCES
    tests/codegen/shift-rotate/test.c
    tests/codegen/shift-rotate/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/shift-rotate/expected.nm
)

c166_add_test(
  NAME codegen.conditional-transfers
  SOURCES
    tests/codegen/conditional-transfers/test.c
    tests/codegen/conditional-transfers/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  SIMULATOR_TIMEOUT 180
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/conditional-transfers/expected.nm
)

c166_add_test(
  NAME codegen.near-call-stack
  SOURCES
    tests/codegen/near-call-stack/test.c
    tests/codegen/near-call-stack/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/near-call-stack/expected.nm
)

c166_add_test(
  NAME codegen.external-branch-relaxation
  SOURCES
    tests/codegen/external-branch-relaxation/test.c
    tests/codegen/external-branch-relaxation/branches.S
    tests/codegen/external-branch-relaxation/targets.S
  MODELS large medium small tiny huge
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/external-branch-relaxation/expected.nm
  LLVM_DIS_CHECK tests/codegen/external-branch-relaxation/expected.dis
)

c166_add_test(
  NAME codegen.backend-tuple-overlap
  SOURCES
    tests/codegen/backend-tuple-overlap/test.c
    tests/codegen/backend-tuple-overlap/probe.S
    tests/codegen/backend-tuple-overlap/anchor.c
    tests/codegen/backend-tuple-overlap/tuple.mir
  MODELS large small huge
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/backend-tuple-overlap/layout.ld
  LLVM_NM_CHECK tests/codegen/backend-tuple-overlap/expected.nm
  LLVM_DIS_CHECK tests/codegen/backend-tuple-overlap/expected.dis
)

c166_add_test(
  NAME codegen.trap
  SOURCES
    tests/codegen/trap/test.c
    tests/codegen/trap/test.S
    tests/codegen/trap/vectors.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/trap/layout.ld
  LLVM_NM_CHECK tests/codegen/trap/expected.nm
)

c166_add_test(
  NAME codegen.code-segment-boundaries
  SOURCES
    tests/codegen/code-segment-boundaries/test.c
    tests/codegen/code-segment-boundaries/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/code-segment-boundaries/layout.ld
  LLVM_NM_CHECK tests/codegen/code-segment-boundaries/expected.nm
)

c166_add_test(
  NAME codegen.extended-addressing
  SOURCES
    tests/codegen/extended-addressing/test.c
    tests/codegen/extended-addressing/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/extended-addressing/layout.ld
  LLVM_NM_CHECK tests/codegen/extended-addressing/expected.nm
)

c166_add_test(
  NAME codegen.esfr-addressing
  SOURCES
    tests/codegen/esfr-addressing/test.c
    tests/codegen/esfr-addressing/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/esfr-addressing/expected.nm
)

c166_add_test(
  NAME codegen.alu-direct-memory
  SOURCES
    tests/codegen/alu-direct-memory/test.c
    tests/codegen/alu-direct-memory/test.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/codegen/alu-direct-memory/layout.ld
  LLVM_NM_CHECK tests/codegen/alu-direct-memory/expected.nm
)

c166_add_test(
  NAME codegen.c-assembly
  SOURCES
    tests/codegen/c-assembly/test.c
    tests/codegen/c-assembly/kernel.c
    tests/codegen/c-assembly/stage.S
  MODELS large medium small tiny huge
  PLATFORMS c167 super10
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_NM_CHECK tests/codegen/c-assembly/expected.nm
)

c166_add_test(
  NAME differential.callback-state-machine
  SOURCES
    tests/differential/callback-state-machine/test.c
    tests/differential/callback-state-machine/machine.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O2 Oz
  CASE_COUNT 16
  SEED 358
  EXPECTED_SIGNATURE 3733987337
  LLVM_NM_CHECK tests/differential/callback-state-machine/expected.nm
)

c166_add_test(
  NAME differential.far-pointer-alias-matrix
  SOURCES
    tests/differential/far-pointer-alias-matrix/test.c
    tests/differential/far-pointer-alias-matrix/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/differential/far-pointer-alias-matrix/expected.nm
)

c166_add_test(
  NAME differential.stream-parser
  SOURCES
    tests/differential/stream-parser/test.c
    tests/differential/stream-parser/stream.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O2 Oz
  CASE_COUNT 20
  SEED 358
  EXPECTED_SIGNATURE 3403786079
  LLVM_NM_CHECK tests/differential/stream-parser/expected.nm
)

c166_add_test(
  NAME differential.record-codec
  SOURCES
    tests/differential/record-codec/test.c
    tests/differential/record-codec/record.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O2 Oz
  CASE_COUNT 28
  SEED 358
  EXPECTED_SIGNATURE 3890913214
  LLVM_NM_CHECK tests/differential/record-codec/expected.nm
)

c166_add_test(
  NAME differential.memory-builtins
  SOURCES tests/differential/memory-builtins/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 16
  SEED 358
  EXPECTED_SIGNATURE 2185321977
  LLVM_LINKER_SCRIPT_BY_MODEL
    large=tests/differential/memory-builtins/layout.ld
    medium=tests/differential/memory-builtins/layout.ld
    huge=tests/differential/memory-builtins/layout.ld
  LLVM_NM_CHECK tests/differential/memory-builtins/expected.nm
)

c166_add_test(
  NAME runtime.runtime-helper-matrix
  SOURCES
    tests/runtime/runtime-helper-matrix/test.c
    tests/runtime/runtime-helper-matrix/eval.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O2
  LLVM_ONLY
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/runtime/runtime-helper-matrix/layout-near.ld
    tiny=tests/runtime/runtime-helper-matrix/layout-near.ld
  LLVM_NM_CHECK tests/runtime/runtime-helper-matrix/expected.nm
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/runtime/runtime-helper-matrix/expected.dis.medium
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME differential.pseudo-expansion-matrix
  SOURCES tests/differential/pseudo-expansion-matrix/test.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/differential/pseudo-expansion-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/pseudo-expansion-matrix/expected.dis
)

c166_add_test(
  NAME codegen.protocol
  SOURCES
    tests/codegen/protocol/test.c
    tests/codegen/protocol/helper.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/codegen/protocol/expected.nm
)

c166_add_test(
  NAME abi.stack-stop
  SOURCES tests/abi/abi-stack-stop/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-stack-stop/expected.nm
)

c166_add_test(
  NAME abi.varargs
  SOURCES tests/abi/abi-varargs/test.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/abi/abi-varargs/expected.nm
)

c166_add_test(
  NAME differential.float64-snapshot
  SOURCES
    tests/differential/float64-snapshot/test.c
    tests/differential/float64-snapshot/memory.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/differential/float64-snapshot/expected.nm
)

c166_add_test(
  NAME differential.scalar-float-bytes
  SOURCES
    tests/differential/scalar-float-bytes/test.c
    tests/differential/scalar-float-bytes/memory.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  CASE_COUNT 10
  SEED 375390536
  EXPECTED_SIGNATURE 1800838867
  LLVM_NM_CHECK tests/differential/scalar-float-bytes/expected.nm
)

set(ieee_operation_id 0)
foreach(ieee_operation IN ITEMS add sub mul div)
  c166_add_test(
    NAME differential.ieee754-f64-${ieee_operation}
    SOURCES
      tests/differential/float64-arithmetic-matrix/test.c
      support/fp_bits.c
    DEFINITIONS C166_IEEE_OPERATION=${ieee_operation_id}
    MODELS large medium small tiny huge
    OPTIMIZATIONS O0 O1 O2 O3 Os Oz
    SIMULATOR_TIMEOUT 180
    LLVM_NM_CHECK tests/differential/float64-arithmetic-matrix/expected.nm
  )
  math(EXPR ieee_operation_id "${ieee_operation_id} + 1")
endforeach()

c166_add_test(
  NAME differential.float64-conversion-matrix
  SOURCES
    tests/differential/float64-conversion-matrix/test.c
    support/fp_bits.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 300
  LLVM_NM_CHECK tests/differential/float64-conversion-matrix/expected.nm
)

c166_add_test(
  NAME differential.float64-storage-matrix
  LLVM_ONLY
  SOURCES
    tests/differential/float64-storage-matrix/test.c
    tests/differential/float64-storage-matrix/eval.c
    tests/differential/float64-storage-matrix/storage.c
    support/fp_bits.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 300
  LLVM_NM_CHECK tests/differential/float64-storage-matrix/expected.nm
)

c166_add_test(
  NAME differential.float32-storage-matrix
  LLVM_ONLY
  SOURCES
    tests/differential/float32-storage-matrix/test.c
    tests/differential/float32-storage-matrix/eval.c
    tests/differential/float32-storage-matrix/storage.c
    support/fp_bits.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 180
  LLVM_NM_CHECK tests/differential/float32-storage-matrix/expected.nm
)

c166_add_test(
  NAME differential.float32-conversion-matrix
  SOURCES
    tests/differential/float32-conversion-matrix/test.c
    tests/differential/float32-conversion-matrix/eval.c
    support/fp_bits.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 300
  LLVM_NM_CHECK tests/differential/float32-conversion-matrix/expected.nm
)

set(ieee_operation_id 0)
foreach(ieee_operation IN ITEMS add sub mul div)
  c166_add_test(
    NAME differential.ieee754-f32-${ieee_operation}
    SOURCES
      tests/differential/float32-arithmetic-matrix/test.c
      tests/differential/float32-arithmetic-matrix/eval.c
      tests/differential/float32-arithmetic-matrix/calls.c
      tests/differential/float32-arithmetic-matrix/storage.c
      support/fp_bits.c
    DEFINITIONS C166_IEEE_OPERATION=${ieee_operation_id}
    MODELS large medium small tiny huge
    OPTIMIZATIONS O0 O1 O2 O3 Os Oz
    SIMULATOR_TIMEOUT 180
    LLVM_NM_CHECK tests/differential/float32-arithmetic-matrix/expected.nm
    LLVM_NM_CHECK_BY_MODEL
      medium=tests/differential/float32-arithmetic-matrix/expected.nm.near
      tiny=tests/differential/float32-arithmetic-matrix/expected.nm.near
    LLVM_DIS_CHECK tests/differential/float32-arithmetic-matrix/expected.dis
    LLVM_DIS_CHECK_BY_MODEL
      medium=tests/differential/float32-arithmetic-matrix/expected.dis.near
      tiny=tests/differential/float32-arithmetic-matrix/expected.dis.near
  )
  math(EXPR ieee_operation_id "${ieee_operation_id} + 1")
endforeach()

c166_add_test(
  NAME differential.float-volatile-matrix
  SOURCES
    tests/differential/float-volatile-matrix/test.c
    tests/differential/float-volatile-matrix/eval.c
    tests/differential/float-volatile-matrix/storage.c
    support/fp_bits.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 300
  LLVM_NM_CHECK tests/differential/float-volatile-matrix/expected.nm
  LLVM_DIS_CHECK tests/differential/float-volatile-matrix/expected.dis
)

c166_add_test(
  NAME differential.generated-type-semantics
  SOURCES
    tests/differential/generated-type-semantics/test.c
    tests/differential/generated-type-semantics/kernels.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 300
  LLVM_NM_CHECK tests/differential/generated-type-semantics/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/differential/generated-type-semantics/expected.nm.near
    tiny=tests/differential/generated-type-semantics/expected.nm.near
  LLVM_DIS_CHECK tests/differential/generated-type-semantics/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/differential/generated-type-semantics/expected.dis.near
    small=tests/differential/generated-type-semantics/expected.dis.small
    tiny=tests/differential/generated-type-semantics/expected.dis.tiny
    huge=tests/differential/generated-type-semantics/expected.dis.huge
)

c166_add_test(
  NAME differential.metamorphic-conversion-matrix
  SOURCES
    tests/differential/metamorphic-conversion-matrix/test.c
    tests/differential/metamorphic-conversion-matrix/eval.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  SIMULATOR_TIMEOUT 75
  LLVM_NM_CHECK tests/differential/metamorphic-conversion-matrix/expected.nm
)

c166_add_test(
  NAME stress.generated-cfg-pressure
  LLVM_ONLY
  SOURCES
    tests/stress/generated-cfg-pressure/test.c
    tests/stress/generated-cfg-pressure/llvm.c
    tests/stress/generated-cfg-pressure/selector.c
  MODELS large small
  OPTIMIZATIONS O2
  LLVM_COMPILE_OPTIONS -mllvm -stress-regalloc=5
  LLVM_LINKER_SCRIPT tests/stress/generated-cfg-pressure/layout.ld
  LLVM_NM_CHECK tests/stress/generated-cfg-pressure/expected.nm
  LLVM_DIS_CHECK tests/stress/generated-cfg-pressure/expected.dis
  SIMULATOR_TIMEOUT 300
)

c166_add_test(
  NAME stress.generated-cfg-pressure-basic
  LLVM_ONLY
  SOURCES
    tests/stress/generated-cfg-pressure/test.c
    tests/stress/generated-cfg-pressure/llvm.c
    tests/stress/generated-cfg-pressure/selector.c
  MODELS large
  OPTIMIZATIONS O2
  LLVM_COMPILE_OPTIONS
    -mllvm -stress-regalloc=5
    -mllvm -regalloc=basic
  LLVM_LINKER_SCRIPT tests/stress/generated-cfg-pressure/layout.ld
  LLVM_NM_CHECK tests/stress/generated-cfg-pressure/expected.nm
  LLVM_DIS_CHECK tests/stress/generated-cfg-pressure/expected.dis
  SIMULATOR_TIMEOUT 300
)

c166_add_test(
  NAME stress.seeded-abi-call
  SOURCES
    tests/stress/seeded-abi-call/test.c
    tests/abi/abi-mixed-slot-matrix/callee.c
    tests/abi/abi-mixed-slot-matrix/caller.c
  MODELS large small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-mixed-slot-matrix/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    small=tests/abi/abi-mixed-slot-matrix/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-mixed-slot-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    small=tests/abi/abi-mixed-slot-matrix/expected.nm.near
  LLVM_DIS_CHECK tests/stress/seeded-abi-call/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    small=tests/stress/seeded-abi-call/expected.dis.small
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME stress.seeded-far-pointer
  SOURCES
    tests/stress/seeded-far-pointer/test.c
    tests/stress/seeded-far-pointer/operations.c
  MODELS large
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/stress/seeded-far-pointer/layout.ld
  LLVM_NM_CHECK tests/stress/seeded-far-pointer/expected.nm
  LLVM_DIS_CHECK tests/stress/seeded-far-pointer/expected.dis
  SIMULATOR_TIMEOUT 300
)

c166_add_test(
  NAME stress.seeded-integer-differential
  SOURCES
    tests/stress/seeded-integer-differential/test.c
    tests/stress/seeded-integer-differential/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_NM_CHECK tests/stress/seeded-integer-differential/expected.nm
  LLVM_DIS_CHECK tests/stress/seeded-integer-differential/expected.dis
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME stress.seeded-f32-to-i32
  SOURCES
    tests/stress/seeded-f32-to-i32/test.c
    tests/stress/seeded-f32-to-i32/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/stress/seeded-f32-to-i32/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/stress/seeded-f32-to-i32/layout-near.ld
    small=tests/stress/seeded-f32-to-i32/layout-small.ld
  LLVM_NM_CHECK tests/stress/seeded-f32-to-i32/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/stress/seeded-f32-to-i32/expected.nm.near
    small=tests/stress/seeded-f32-to-i32/expected.nm.small
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME stress.seeded-f32-differential
  SOURCES
    tests/stress/seeded-f32-differential/test.c
    tests/stress/seeded-f32-differential/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/stress/seeded-f32-differential/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/stress/seeded-f32-differential/layout-near.ld
    small=tests/stress/seeded-f32-differential/layout-small.ld
  LLVM_NM_CHECK tests/stress/seeded-f32-differential/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/stress/seeded-f32-differential/expected.nm.near
    small=tests/stress/seeded-f32-differential/expected.nm.small
  LLVM_DIS_CHECK tests/stress/seeded-f32-differential/expected.dis
  LLVM_DIS_CHECK_MODELS medium
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME stress.seeded-f64-to-f32
  SOURCES
    tests/stress/seeded-f64-to-f32/test.c
    tests/stress/seeded-f64-to-f32/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/stress/seeded-f64-to-f32/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/stress/seeded-f64-to-f32/layout-near.ld
    small=tests/stress/seeded-f64-to-f32/layout-small.ld
  LLVM_NM_CHECK tests/stress/seeded-f64-to-f32/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/stress/seeded-f64-to-f32/expected.nm.near
    small=tests/stress/seeded-f64-to-f32/expected.nm.small
  LLVM_DIS_CHECK tests/stress/seeded-f64-to-f32/expected.dis
  LLVM_DIS_CHECK_MODELS medium
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME stress.seeded-f64-differential
  SOURCES
    tests/stress/seeded-f64-differential/test.c
    tests/stress/seeded-f64-differential/operations.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/stress/seeded-f64-differential/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/stress/seeded-f64-differential/layout-near.ld
    small=tests/stress/seeded-f64-differential/layout-small.ld
  LLVM_NM_CHECK tests/stress/seeded-f64-differential/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/stress/seeded-f64-differential/expected.nm.near
    small=tests/stress/seeded-f64-differential/expected.nm.small
  LLVM_DIS_CHECK tests/stress/seeded-f64-differential/expected.dis
  LLVM_DIS_CHECK_MODELS medium
  SIMULATOR_TIMEOUT 600
)

c166_add_test(
  NAME abi.abi-mixed-recursion
  SOURCES
    tests/abi/abi-mixed-recursion/test.c
    tests/abi/abi-mixed-recursion/recursion-a.c
    tests/abi/abi-mixed-recursion/recursion-b.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-mixed-recursion/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-mixed-recursion/layout-near.ld
    small=tests/abi/abi-mixed-recursion/layout-small.ld
    tiny=tests/abi/abi-mixed-recursion/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-mixed-recursion/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-mixed-recursion/expected.nm.near
    small=tests/abi/abi-mixed-recursion/expected.nm.small
    tiny=tests/abi/abi-mixed-recursion/expected.nm.small
  LLVM_DIS_CHECK tests/abi/abi-mixed-recursion/expected.dis
  LLVM_DIS_CHECK_MODELS medium
  SIMULATOR_TIMEOUT 150
)

c166_add_test(
  NAME abi.abi-varargs-aggregate
  SOURCES
    tests/abi/abi-varargs-aggregate/test.c
    tests/abi/abi-varargs-aggregate/callee.c
    tests/abi/abi-varargs-aggregate/caller.c
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-varargs-aggregate/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-varargs-aggregate/layout-near.ld
    small=tests/abi/abi-varargs-aggregate/layout-small.ld
    tiny=tests/abi/abi-varargs-aggregate/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-varargs-aggregate/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-varargs-aggregate/expected.nm.near
    small=tests/abi/abi-varargs-aggregate/expected.nm.small
    tiny=tests/abi/abi-varargs-aggregate/expected.nm.small
  LLVM_DIS_CHECK tests/abi/abi-varargs-aggregate/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-varargs-aggregate/expected.dis.medium
  SIMULATOR_TIMEOUT 90
)

c166_add_test(
  NAME abi.abi-varargs-matrix
  SOURCES
    tests/abi/abi-varargs-matrix/test.c
    tests/abi/abi-varargs-matrix/callee.c
    tests/abi/abi-varargs-matrix/caller.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-varargs-matrix/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-varargs-matrix/layout-near.ld
    small=tests/abi/abi-varargs-matrix/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-varargs-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-varargs-matrix/expected.nm.near
    small=tests/abi/abi-varargs-matrix/expected.nm.small
  LLVM_DIS_CHECK tests/abi/abi-varargs-matrix/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-varargs-matrix/expected.dis.medium
    small=tests/abi/abi-varargs-matrix/expected.dis.small
  SIMULATOR_TIMEOUT 120
)

c166_add_test(
  NAME abi.abi-mixed-slot-matrix
  SOURCES
    tests/abi/abi-mixed-slot-matrix/test.c
    tests/abi/abi-mixed-slot-matrix/callee.c
    tests/abi/abi-mixed-slot-matrix/caller.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/abi/abi-mixed-slot-matrix/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-mixed-slot-matrix/layout-near.ld
    small=tests/abi/abi-mixed-slot-matrix/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-mixed-slot-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-mixed-slot-matrix/expected.nm.near
    small=tests/abi/abi-mixed-slot-matrix/expected.nm.near
  LLVM_DIS_CHECK tests/abi/abi-mixed-slot-matrix/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/abi/abi-mixed-slot-matrix/expected.dis.medium
    small=tests/abi/abi-mixed-slot-matrix/expected.dis.small
  SIMULATOR_TIMEOUT 120
)

c166_add_test(
  NAME abi.abi-code-bank
  SOURCES
    tests/abi/abi-code-bank/llvm.c
    tests/abi/abi-code-bank/banksw.S
  MODELS large small
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_ONLY
  LLVM_LINKER_SCRIPT tests/abi/abi-code-bank/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    small=tests/abi/abi-code-bank/layout-small.ld
  LLVM_NM_CHECK tests/abi/abi-code-bank/expected.nm
  LLVM_DIS_CHECK tests/abi/abi-code-bank/expected.dis
  SIMULATOR_TIMEOUT 60
)

set(C166_LINKED_APPLICATION_SOURCES
  tests/abi/abi-linked-application/test.c
  tests/abi/abi-linked-application/entry.c
  tests/abi/abi-linked-application/table.c
  tests/abi/abi-linked-application/common-update.c
  tests/abi/abi-linked-application/common-observe.c
  tests/abi/abi-linked-application/weak.c
  tests/abi/abi-linked-application/strong.c
)
set(C166_LINKED_APPLICATION_ARCHIVE_SOURCES
  tests/abi/abi-linked-application/archive-live.c
  tests/abi/abi-linked-application/archive-unused.c
)

c166_add_test(
  NAME abi.abi-linked-application
  LLVM_ONLY
  SOURCES ${C166_LINKED_APPLICATION_SOURCES}
  LLVM_ARCHIVE_SOURCES ${C166_LINKED_APPLICATION_ARCHIVE_SOURCES}
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O2 Oz
  LLVM_COMPILE_OPTIONS -ffunction-sections -fdata-sections -fcommon
  LLVM_LINK_OPTIONS --gc-sections
  LLVM_LINK_OPTIONS_BY_MODEL
    large=--section-start=.bss=0x418000
    medium=--section-start=.bss=0x418000
    small=--section-start=.bss=0x5800
    tiny=--section-start=.bss=0x5800
    huge=--section-start=.bss=0x418000
  LLVM_LINKER_SCRIPT tests/abi/abi-linked-application/layout-large.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-linked-application/layout-medium.ld
    small=tests/abi/abi-linked-application/layout-small.ld
    tiny=tests/abi/abi-linked-application/layout-tiny.ld
  LLVM_NM_CHECK tests/abi/abi-linked-application/expected.nm.large
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-linked-application/expected.nm.medium
    small=tests/abi/abi-linked-application/expected.nm.small
    tiny=tests/abi/abi-linked-application/expected.nm.tiny
  LLVM_NM_FORBID tests/abi/abi-linked-application/expected.nm.forbid
  SIMULATOR_TIMEOUT 60
)

c166_add_test(
  NAME abi.abi-linked-application-relocated
  LLVM_ONLY
  SOURCES ${C166_LINKED_APPLICATION_SOURCES}
  LLVM_ARCHIVE_SOURCES ${C166_LINKED_APPLICATION_ARCHIVE_SOURCES}
  MODELS large medium small tiny huge
  OPTIMIZATIONS O0 O2 Oz
  LLVM_COMPILE_OPTIONS -ffunction-sections -fdata-sections -fcommon
  LLVM_LINK_OPTIONS --gc-sections
  LLVM_LINK_OPTIONS_BY_MODEL
    large=--section-start=.bss=0x428000
    medium=--section-start=.bss=0x428000
    small=--section-start=.bss=0x6800
    tiny=--section-start=.bss=0x6800
    huge=--section-start=.bss=0x428000
  LLVM_LINKER_SCRIPT
    tests/abi/abi-linked-application-relocated/layout-large.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/abi/abi-linked-application-relocated/layout-medium.ld
    small=tests/abi/abi-linked-application-relocated/layout-small.ld
    tiny=tests/abi/abi-linked-application-relocated/layout-tiny.ld
  LLVM_NM_CHECK
    tests/abi/abi-linked-application-relocated/expected.nm.large
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/abi/abi-linked-application-relocated/expected.nm.medium
    small=tests/abi/abi-linked-application-relocated/expected.nm.small
    tiny=tests/abi/abi-linked-application-relocated/expected.nm.tiny
  LLVM_NM_FORBID tests/abi/abi-linked-application/expected.nm.forbid
  SIMULATOR_TIMEOUT 60
)

c166_add_test(
  NAME differential.float-varargs-matrix
  SOURCES
    tests/differential/float-varargs-matrix/test.c
    tests/differential/float-varargs-matrix/callee.c
    tests/differential/float-varargs-matrix/caller.c
  MODELS large medium small huge
  OPTIMIZATIONS O0 O1 O2 O3 Os Oz
  LLVM_LINKER_SCRIPT tests/differential/float-varargs-matrix/layout.ld
  LLVM_LINKER_SCRIPT_BY_MODEL
    medium=tests/differential/float-varargs-matrix/layout-near.ld
    small=tests/differential/float-varargs-matrix/layout-small.ld
  LLVM_NM_CHECK tests/differential/float-varargs-matrix/expected.nm
  LLVM_NM_CHECK_BY_MODEL
    medium=tests/differential/float-varargs-matrix/expected.nm.near
    small=tests/differential/float-varargs-matrix/expected.nm.small
  LLVM_DIS_CHECK tests/differential/float-varargs-matrix/expected.dis
  LLVM_DIS_CHECK_BY_MODEL
    medium=tests/differential/float-varargs-matrix/expected.dis.medium
    small=tests/differential/float-varargs-matrix/expected.dis.small
  SIMULATOR_TIMEOUT 300
)

c166_add_test(
  NAME differential.code-size-corpus
  SOURCES
    tests/differential/code-size-corpus/test.c
    ${C166_SIZE_INTEGER_SOURCES}
  MODELS large medium small tiny huge
  OPTIMIZATIONS Os
  CASE_COUNT 60
  SEED 375390532
  EXPECTED_SIGNATURE 2520872449
  LLVM_NM_CHECK tests/differential/code-size-corpus/expected.nm
  SIMULATOR_TIMEOUT 240
)

c166_add_test(
  NAME differential.code-size-floating
  SOURCES
    tests/differential/code-size-corpus/test.c
    ${C166_SIZE_FLOAT_SOURCES}
  MODELS large medium small tiny huge
  OPTIMIZATIONS Os
  DEFINITIONS SIZE_CORPUS_FLOATING_ONLY=1
  CASE_COUNT 10
  SEED 375390533
  EXPECTED_SIGNATURE 3661311263
  LLVM_NM_CHECK tests/differential/code-size-floating/expected.nm
  SIMULATOR_TIMEOUT 240
)

c166_add_size_comparisons()
c166_add_runtime_closure_tests()

get_property(c166_test_build_targets GLOBAL PROPERTY C166_TEST_BUILD_TARGETS)
add_custom_target(c166-tests-build DEPENDS ${c166_test_build_targets})
