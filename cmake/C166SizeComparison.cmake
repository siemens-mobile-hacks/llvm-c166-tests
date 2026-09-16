set(C166_SIZE_INTEGER_SOURCES
  size/benchmarks/aggregate-arguments.c
  size/benchmarks/aggregate-return.c
  size/benchmarks/bitfields.c
  size/benchmarks/bit-booleans.c
  size/benchmarks/byte-arithmetic.c
  size/benchmarks/byte-extensions.c
  size/benchmarks/byte-indexed-loop.c
  size/benchmarks/callee-saved.c
  size/benchmarks/conditional-branches.c
  size/benchmarks/far-pointers.c
  size/benchmarks/frame-address-pressure.c
  size/benchmarks/global-memory.c
  size/benchmarks/immediates.c
  size/benchmarks/indexed-loop.c
  size/benchmarks/indirect-call.c
  size/benchmarks/long-arithmetic.c
  size/benchmarks/long-compare.c
  size/benchmarks/long-indexed-loop.c
  size/benchmarks/long-shifts.c
  size/benchmarks/multiply-divide.c
  size/benchmarks/outgoing-stack.c
  size/benchmarks/path-redirect.c
  size/benchmarks/readonly-data.c
  size/benchmarks/stack-address.c
  size/benchmarks/stack-arguments.c
  size/benchmarks/switch-dense.c
  size/benchmarks/switch-sparse.c
  size/benchmarks/tail-call.c
  size/benchmarks/varargs.c
)

set(C166_SIZE_FLOAT_SOURCES
  size/benchmarks/double-arithmetic.c
  size/benchmarks/double-comparison.c
  size/benchmarks/float-arithmetic.c
  size/benchmarks/integer-to-double.c
)

function(_c166_add_size_comparison model)
  if(model STREQUAL "large")
    set(tasking_model -Ml)
  elseif(model STREQUAL "medium")
    set(tasking_model -Mm)
  elseif(model STREQUAL "small")
    set(tasking_model -Ms)
  elseif(model STREQUAL "tiny")
    set(tasking_model -Mt)
  elseif(model STREQUAL "huge")
    set(tasking_model -Mh)
  else()
    message(FATAL_ERROR "unsupported C166 memory model: ${model}")
  endif()

  set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/size/${model}")
  set(report_dir "${CMAKE_CURRENT_BINARY_DIR}/reports/${model}")
  set(outputs)
  foreach(relative_source IN LISTS C166_SIZE_INTEGER_SOURCES C166_SIZE_FLOAT_SOURCES)
    set(source "${C166_TEST_ROOT}/${relative_source}")
    get_filename_component(name "${source}" NAME_WE)
    set(case_dir "${build_dir}/${name}")
    set(llvm_object "${case_dir}/${name}.llvm.o")
    set(llvm_size "${case_dir}/${name}.llvm.size")
    set(tasking_object "${case_dir}/${name}.tasking.obj")
    set(tasking_source "${case_dir}/${name}.src")
    set(tasking_listing "${case_dir}/${name}.lst")
    set(tasking_reassembled "${case_dir}/${name}.reassembled.obj")
    _c166_wine_path(wine_source "${source}")
    file(MAKE_DIRECTORY "${case_dir}")

    add_custom_command(
      OUTPUT "${llvm_size}" "${tasking_listing}"
      BYPRODUCTS "${llvm_object}" "${tasking_object}" "${tasking_source}"
        "${tasking_reassembled}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${case_dir}"
      COMMAND "${C166_CLANG}" --target=c166-none-elf "-mcmodel=${model}"
        -Os -ffreestanding -fno-builtin -ffunction-sections -fdata-sections
        -c "${source}" -o "${llvm_object}"
      COMMAND "${CMAKE_COMMAND}"
        "-DTOOL=${C166_SIZE}" -DOPTION=-A
        "-DINPUT=${llvm_object}" "-DOUTPUT=${llvm_size}"
        -P "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
      COMMAND "${CMAKE_COMMAND}" -E env
        "WINEPREFIX=${C166_WINEPREFIX}"
        "${C166_TEST_ROOT}/tools/with-noninteractive-wine"
        wine "${C166_TASKING_CC}" "${tasking_model}" -O2
        -RclFC=SIZE_DATA -RclPR=SIZE_TEXT -tmp
        -c -o "${name}.tasking.obj" "${wine_source}"
      COMMAND "${CMAKE_COMMAND}" -E env
        "WINEPREFIX=${C166_WINEPREFIX}"
        "${C166_TEST_ROOT}/tools/with-noninteractive-wine"
        wine "${C166_TASKING_AS}" "${name}.src"
        TO "${name}.reassembled.obj"
      WORKING_DIRECTORY "${case_dir}"
      DEPENDS "${source}" "${C166_CLANG}" "${C166_SIZE}"
        "${C166_TASKING_CC}" "${C166_TASKING_AS}"
        "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
        "${C166_TEST_ROOT}/tools/with-noninteractive-wine"
      VERBATIM
    )
    list(APPEND outputs "${llvm_size}" "${tasking_listing}")
  endforeach()

  set(report_tsv "${report_dir}/code-size.tsv")
  set(report_md "${report_dir}/code-size.md")
  add_custom_command(
    OUTPUT "${report_tsv}" "${report_md}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${report_dir}"
    COMMAND "${C166_TEST_ROOT}/tools/generate-code-size-report"
      "${model}" "${build_dir}" "${report_tsv}" "${report_md}"
    DEPENDS ${outputs} "${C166_TEST_ROOT}/tools/generate-code-size-report"
    VERBATIM
  )

  set(target "c166-size-${model}-codegen-corpus")
  add_custom_target("${target}" DEPENDS "${report_tsv}" "${report_md}")
  set_property(GLOBAL APPEND PROPERTY C166_TEST_BUILD_TARGETS "${target}")
  set(test "size.${model}.codegen.corpus")
  add_test(
    NAME "${test}"
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
      --target "${target}" --parallel "${C166_TEST_JOBS}"
  )
  set_tests_properties("${test}" PROPERTIES
    LABELS "size;comparison;codegen;${model}"
    RESOURCE_LOCK c166_cmake_build
    TIMEOUT 300
  )
endfunction()

function(c166_add_size_comparisons)
  foreach(model IN ITEMS large medium small tiny huge)
    _c166_add_size_comparison("${model}")
  endforeach()
endfunction()

function(_c166_add_runtime_closure model)
  execute_process(
    COMMAND "${C166_CLANG}" --target=c166-none-elf "-mcmodel=${model}"
      --print-libgcc-file-name
    OUTPUT_VARIABLE runtime_archive
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE runtime_result)
  if(NOT runtime_result EQUAL 0 OR NOT EXISTS "${runtime_archive}")
    message(FATAL_ERROR
      "missing compiler runtime for ${model}: ${runtime_archive}")
  endif()

  set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/runtime-closure/${model}")
  set(source "${C166_TEST_ROOT}/size/runtime-closure.c")
  set(limits "${C166_TEST_ROOT}/size/runtime-closures.tsv")
  file(STRINGS "${limits}" rows)
  set(outputs)
  foreach(row IN LISTS rows)
    if(row MATCHES "^helper[	]")
      continue()
    endif()
    string(REPLACE "	" ";" fields "${row}")
    list(GET fields 0 helper)
    list(GET fields 1 selector)
    set(object "${build_dir}/${helper}.o")
    set(executable "${build_dir}/${helper}.elf")
    set(size_file "${build_dir}/${helper}.size")
    add_custom_command(
      OUTPUT "${size_file}"
      BYPRODUCTS "${object}" "${executable}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${build_dir}"
      COMMAND "${C166_CLANG}" --target=c166-none-elf "-mcmodel=${model}"
        -Os -ffreestanding -fno-builtin -ffunction-sections -fdata-sections
        -Wno-atomic-alignment "-DC166_CLOSURE_HELPER=${selector}"
        -c "${source}" -o "${object}"
      COMMAND "${C166_LLD}" -m c166elf --gc-sections
        --entry=_runtime_closure_root
        --section-start=.runtime_closure_root=0x1000
        --section-start=.c166.near.text=0x2000
        --section-start=.c166.small.rodata=0x3000
        -Ttext=0x10000 "${object}" "${runtime_archive}" -o "${executable}"
      COMMAND "${CMAKE_COMMAND}"
        "-DTOOL=${C166_SIZE}" -DOPTION=-A
        "-DINPUT=${executable}" "-DOUTPUT=${size_file}"
        -P "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
      DEPENDS "${source}" "${limits}" "${runtime_archive}"
        "${C166_CLANG}" "${C166_LLD}" "${C166_SIZE}"
        "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
      VERBATIM)
    list(APPEND outputs "${size_file}")
  endforeach()

  set(target "c166-runtime-closure-${model}")
  add_custom_target("${target}" DEPENDS ${outputs})
  set_property(GLOBAL APPEND PROPERTY C166_TEST_BUILD_TARGETS "${target}")
  set(fixture "fixture-runtime-closure-${model}")
  set(build_test "build.${model}.runtime.closure")
  add_test(
    NAME "${build_test}"
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
      --target "${target}" --parallel "${C166_TEST_JOBS}")
  set_tests_properties("${build_test}" PROPERTIES
    FIXTURES_SETUP "${fixture}"
    LABELS "build;size;runtime;closure;${model}"
    RESOURCE_LOCK c166_cmake_build
    TIMEOUT 120)

  set(check_test "size.${model}.runtime.closure")
  add_test(
    NAME "${check_test}"
    COMMAND "${CMAKE_COMMAND}"
      "-DMODEL=${model}" "-DINPUT_DIR=${build_dir}" "-DLIMITS=${limits}"
      -P "${C166_TEST_ROOT}/cmake/CheckRuntimeClosures.cmake")
  set_tests_properties("${check_test}" PROPERTIES
    FIXTURES_REQUIRED "${fixture}"
    LABELS "size;runtime;closure;${model}")
endfunction()

function(c166_add_runtime_closure_tests)
  foreach(model IN ITEMS large medium small tiny huge)
    _c166_add_runtime_closure("${model}")
  endforeach()
endfunction()
