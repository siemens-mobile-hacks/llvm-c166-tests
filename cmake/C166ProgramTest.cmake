include(CMakeParseArguments)

function(_c166_wine_path output path)
  get_filename_component(absolute "${path}" ABSOLUTE)
  string(REPLACE "/" "\\" windows "${absolute}")
  set(${output} "Z:${windows}" PARENT_SCOPE)
endfunction()

function(_c166_model_value output model fallback)
  set(value "${fallback}")
  set(found FALSE)
  foreach(mapping IN LISTS ARGN)
    string(FIND "${mapping}" "=" separator)
    if(separator LESS 1)
      message(FATAL_ERROR
        "model-specific value must use model=value syntax: ${mapping}")
    endif()
    string(SUBSTRING "${mapping}" 0 ${separator} mapping_model)
    math(EXPR value_start "${separator} + 1")
    string(SUBSTRING "${mapping}" ${value_start} -1 mapping_value)
    if("${mapping_model}" STREQUAL "${model}")
      if(found)
        message(FATAL_ERROR "duplicate value for model ${model}")
      endif()
      set(value "${mapping_value}")
      set(found TRUE)
    endif()
  endforeach()
  set(${output} "${value}" PARENT_SCOPE)
endfunction()

function(_c166_model_values output model)
  set(values)
  foreach(mapping IN LISTS ARGN)
    string(FIND "${mapping}" "=" separator)
    if(separator LESS 1)
      message(FATAL_ERROR
        "model-specific value must use model=value syntax: ${mapping}")
    endif()
    string(SUBSTRING "${mapping}" 0 ${separator} mapping_model)
    if("${mapping_model}" STREQUAL "${model}")
      math(EXPR value_start "${separator} + 1")
      string(SUBSTRING "${mapping}" ${value_start} -1 mapping_value)
      list(APPEND values "${mapping_value}")
    endif()
  endforeach()
  set(${output} "${values}" PARENT_SCOPE)
endfunction()

if(NOT DEFINED ENV{LLVM_BUILD})
  message(FATAL_ERROR "set LLVM_BUILD to the C166 LLVM build directory")
endif()

set(C166_LLVM_BUILD "$ENV{LLVM_BUILD}")
set(C166_CLANG "${C166_LLVM_BUILD}/bin/clang")
set(C166_LLC "${C166_LLVM_BUILD}/bin/llc")
set(C166_LLD "${C166_LLVM_BUILD}/bin/ld.lld")
set(C166_OBJCOPY "${C166_LLVM_BUILD}/bin/llvm-objcopy")
set(C166_OBJDUMP "${C166_LLVM_BUILD}/bin/llvm-objdump")
set(C166_NM "${C166_LLVM_BUILD}/bin/llvm-nm")
set(C166_AR "${C166_LLVM_BUILD}/bin/llvm-ar")
set(C166_SIZE "${C166_LLVM_BUILD}/bin/llvm-size")

if(DEFINED ENV{WINEPREFIX})
  set(C166_WINEPREFIX "$ENV{WINEPREFIX}")
else()
  set(C166_WINEPREFIX "$ENV{HOME}/.wine")
endif()
set(C166_TASKING_ROOT
  "${C166_WINEPREFIX}/drive_c/Program Files (x86)/TASKING/dc166 v8.6")
set(C166_CROSSVIEW "${C166_TASKING_ROOT}/bin/xfw166.exe")
set(C166_TASKING_CC "${C166_TASKING_ROOT}/bin/cc166.exe")
set(C166_TASKING_C "${C166_TASKING_ROOT}/bin/c166.exe")
set(C166_TASKING_AS "${C166_TASKING_ROOT}/bin/a166.exe")
set(C166_TASKING_LD "${C166_TASKING_ROOT}/bin/l166.exe")
set(C166_TASKING_M "${C166_TASKING_ROOT}/bin/m166.exe")
set(C166_CROSSVIEW_INI "${C166_TASKING_ROOT}/etc/xvw.ini")
set(C166_SIMULATOR_C167 "${C166_TASKING_ROOT}/etc/sim167.cfg")
# The ext2mac/Super10 peripheral model does not expose ASC0 (or SSC0) to
# CrossView's DIO terminal.  The F397 EXT22MAC simulator is an instruction-set
# superset and provides the ASC0 transport used by the runner.
set(C166_SIMULATOR_SUPER10 "${C166_TASKING_ROOT}/etc/simf397.cfg")

foreach(tool IN ITEMS
    C166_CLANG C166_LLC C166_LLD C166_OBJCOPY C166_OBJDUMP C166_NM C166_CROSSVIEW
    C166_TASKING_CC C166_TASKING_C C166_TASKING_AS C166_TASKING_LD C166_TASKING_M
    C166_AR C166_SIZE C166_CROSSVIEW_INI C166_SIMULATOR_C167
    C166_SIMULATOR_SUPER10)
  if(NOT EXISTS "${${tool}}")
    message(FATAL_ERROR "missing ${tool}: ${${tool}}")
  endif()
endforeach()

set(C166_TASKING_COMMON_DEPENDENCIES
  "${C166_TASKING_CC}"
  "${C166_TASKING_C}"
  "${C166_TASKING_AS}"
  "${C166_TASKING_LD}"
  "${C166_TASKING_M}"
  "${C166_TASKING_ROOT}/include/_c_init.asm")
file(GLOB C166_TASKING_EXT_LIBRARIES CONFIGURE_DEPENDS
  "${C166_TASKING_ROOT}/lib/ext/*.lib")
file(GLOB C166_TASKING_EXT2_LIBRARIES CONFIGURE_DEPENDS
  "${C166_TASKING_ROOT}/lib/ext2/*.lib")
if(NOT C166_TASKING_EXT_LIBRARIES OR NOT C166_TASKING_EXT2_LIBRARIES)
  message(FATAL_ERROR "missing TASKING runtime libraries")
endif()
set(C166_TASKING_EXT_DEPENDENCIES
  ${C166_TASKING_COMMON_DEPENDENCIES}
  "${C166_TASKING_ROOT}/include/reg167.h"
  "${C166_TASKING_ROOT}/etc/reg167.def"
  "${C166_TASKING_ROOT}/lib/src/cstartx.asm"
  ${C166_TASKING_EXT_LIBRARIES})
set(C166_TASKING_EXT2_DEPENDENCIES
  ${C166_TASKING_COMMON_DEPENDENCIES}
  "${C166_TASKING_ROOT}/include/regsuper10bo.h"
  "${C166_TASKING_ROOT}/etc/regsuper10bo.def"
  "${C166_TASKING_ROOT}/lib/src/cstartx2.asm"
  ${C166_TASKING_EXT2_LIBRARIES})
foreach(dependency IN LISTS
    C166_TASKING_EXT_DEPENDENCIES C166_TASKING_EXT2_DEPENDENCIES)
  if(NOT EXISTS "${dependency}")
    message(FATAL_ERROR "missing TASKING dependency: ${dependency}")
  endif()
endforeach()

foreach(model IN ITEMS large medium small tiny huge)
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
  set("C166_RUNTIME_${model}" "${runtime_archive}")
endforeach()

function(_c166_add_test_variant)
  cmake_parse_arguments(ARG "LLVM_ONLY"
    "NAME;MODEL;PLATFORM;OPTIMIZATION;LLVM_LINKER_SCRIPT;LLVM_NM_CHECK;LLVM_NM_FORBID;LLVM_DIS_CHECK;CASE_COUNT;SEED;EXPECTED_SIGNATURE;SIMULATOR_TIMEOUT"
    "SOURCES;LLVM_ARCHIVE_SOURCES;DEFINITIONS;LLVM_COMPILE_OPTIONS;LLVM_LINK_OPTIONS" ${ARGN})
  foreach(required IN ITEMS NAME MODEL PLATFORM OPTIMIZATION SOURCES)
    if(NOT ARG_${required})
      message(FATAL_ERROR "_c166_add_test_variant requires ${required}")
    endif()
  endforeach()
  if(NOT ARG_SIMULATOR_TIMEOUT)
    set(ARG_SIMULATOR_TIMEOUT 90)
  endif()

  if(ARG_PLATFORM STREQUAL "c167")
    set(simulator_config "${C166_SIMULATOR_C167}")
    set(platform_component "")
    set(tasking_architecture ext)
    set(tasking_arch_flags)
    set(tasking_register_header reg167.h)
    set(tasking_register_file reg167.def)
    set(tasking_toolchain_dependencies ${C166_TASKING_EXT_DEPENDENCIES})
    set(platform_definition C166_TEST_SUPER10=0)
  elseif(ARG_PLATFORM STREQUAL "super10")
    set(simulator_config "${C166_SIMULATOR_SUPER10}")
    set(platform_component "-super10")
    set(tasking_architecture ext2)
    set(tasking_arch_flags -x2)
    set(tasking_register_header regsuper10bo.h)
    set(tasking_register_file regsuper10bo.def)
    set(tasking_toolchain_dependencies ${C166_TASKING_EXT2_DEPENDENCIES})
    set(platform_definition C166_TEST_SUPER10=1)
  else()
    message(FATAL_ERROR "unsupported C166 test platform: ${ARG_PLATFORM}")
  endif()
  math(EXPR ctest_timeout "${ARG_SIMULATOR_TIMEOUT} + 20")

  set(startup "${C166_TEST_ROOT}/support/startup.S")
  if(ARG_MODEL STREQUAL "small" OR ARG_MODEL STREQUAL "tiny")
    set(linker_script "${C166_TEST_ROOT}/support/c166-small.ld")
  else()
    set(linker_script "${C166_TEST_ROOT}/support/c166-large.ld")
  endif()
  if(ARG_MODEL STREQUAL "large")
    set(tasking_model -Ml)
  elseif(ARG_MODEL STREQUAL "medium")
    set(tasking_model -Mm)
  elseif(ARG_MODEL STREQUAL "small")
    set(tasking_model -Ms)
  elseif(ARG_MODEL STREQUAL "tiny")
    set(tasking_model -Mt)
  elseif(ARG_MODEL STREQUAL "huge")
    set(tasking_model -Mh)
  else()
    message(FATAL_ERROR "unsupported C166 memory model: ${ARG_MODEL}")
  endif()

  set(runtime_archive "${C166_RUNTIME_${ARG_MODEL}}")

  string(REPLACE "." "-" target_component "${ARG_NAME}")
  set(output_dir
    "${CMAKE_CURRENT_BINARY_DIR}/programs/${target_component}-${ARG_MODEL}-${ARG_OPTIMIZATION}${platform_component}")
  set(objects)
  set(sources
    "${startup}"
    "${C166_TEST_ROOT}/support/crt.c"
    "${C166_TEST_ROOT}/support/asc0.c"
    "${C166_TEST_ROOT}/support/tap.c")
  set(defines "-D${platform_definition}")
  foreach(definition IN LISTS ARG_DEFINITIONS)
    list(APPEND defines "-D${definition}")
  endforeach()
  if(ARG_MODEL STREQUAL "small")
    list(APPEND defines -DC166_TEST_SMALL=1)
  else()
    list(APPEND defines -DC166_TEST_SMALL=0)
  endif()
  if(ARG_CASE_COUNT)
    foreach(required IN ITEMS SEED EXPECTED_SIGNATURE)
      if(NOT ARG_${required})
        message(FATAL_ERROR
          "differential test ${ARG_NAME} requires ${required}")
      endif()
    endforeach()
    list(APPEND sources "${C166_TEST_ROOT}/support/differential.c")
    list(APPEND defines
      "-DC166_CASE_COUNT=${ARG_CASE_COUNT}"
      "-DC166_SEED=${ARG_SEED}UL"
      "-DC166_EXPECTED_SIGNATURE=${ARG_EXPECTED_SIGNATURE}UL")
  endif()
  set(test_sources)
  foreach(source IN LISTS ARG_SOURCES)
    if(IS_ABSOLUTE "${source}")
      list(APPEND test_sources "${source}")
    else()
      list(APPEND test_sources "${C166_TEST_ROOT}/${source}")
    endif()
  endforeach()
  set(archive_sources)
  foreach(source IN LISTS ARG_LLVM_ARCHIVE_SOURCES)
    if(IS_ABSOLUTE "${source}")
      list(APPEND archive_sources "${source}")
    else()
      list(APPEND archive_sources "${C166_TEST_ROOT}/${source}")
    endif()
  endforeach()
  file(GLOB support_headers CONFIGURE_DEPENDS
    "${C166_TEST_ROOT}/support/include/*.h")
  set(test_dependencies ${support_headers})
  foreach(source IN LISTS test_sources archive_sources)
    get_filename_component(source_directory "${source}" DIRECTORY)
    file(GLOB source_inputs CONFIGURE_DEPENDS
      "${source_directory}/*.h" "${source_directory}/*.inc")
    list(APPEND test_dependencies ${source_inputs})
  endforeach()

  set(archive_objects)
  set(index 0)
  foreach(source IN LISTS archive_sources)
    set(object "${output_dir}/archive/${index}.o")
    get_filename_component(extension "${source}" LAST_EXT)
    if(NOT extension STREQUAL ".c")
      message(FATAL_ERROR "archive source must be C: ${source}")
    endif()
    add_custom_command(
      OUTPUT "${object}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}/archive"
      COMMAND "${C166_CLANG}"
        --target=c166-none-elf "-mcmodel=${ARG_MODEL}"
        "-${ARG_OPTIMIZATION}" -ffreestanding -fno-builtin
        -mllvm -verify-machineinstrs -DC166_TEST_LLVM=1 ${defines}
        -I "${C166_TEST_ROOT}/support/include"
        ${ARG_LLVM_COMPILE_OPTIONS}
        -c "${source}" -o "${object}"
      DEPENDS "${source}" ${test_dependencies} "${C166_CLANG}"
      VERBATIM
    )
    list(APPEND archive_objects "${object}")
    math(EXPR index "${index} + 1")
  endforeach()

  set(test_archive)
  if(archive_objects)
    set(test_archive "${output_dir}/test-archive.a")
    add_custom_command(
      OUTPUT "${test_archive}"
      COMMAND "${CMAKE_COMMAND}" -E rm -f "${test_archive}"
      COMMAND "${C166_AR}" rcs "${test_archive}" ${archive_objects}
      DEPENDS ${archive_objects} "${C166_AR}"
      VERBATIM
    )
  endif()
  list(REMOVE_DUPLICATES test_dependencies)
  list(APPEND sources ${test_sources})
  set(llc_code_model "${ARG_MODEL}")
  if(ARG_MODEL STREQUAL "huge")
    set(llc_code_model large)
  endif()
  set(index 0)
  foreach(source IN LISTS sources)
    set(object "${output_dir}/${index}.o")
    get_filename_component(extension "${source}" LAST_EXT)
    if(extension STREQUAL ".mir")
      set(post_mir "${output_dir}/${index}.post.mir")
      add_custom_command(
        OUTPUT "${object}"
        BYPRODUCTS "${post_mir}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}"
        COMMAND "${C166_LLC}" -mtriple=c166-none-elf
          "-code-model=${llc_code_model}" "-target-abi=${ARG_MODEL}"
          -run-pass=postrapseudos -verify-machineinstrs
          -o "${post_mir}" "${source}"
        COMMAND "${C166_LLC}" -mtriple=c166-none-elf
          "-code-model=${llc_code_model}" "-target-abi=${ARG_MODEL}"
          -start-after=postrapseudos -verify-machineinstrs -filetype=obj
          -o "${object}" "${post_mir}"
        DEPENDS "${source}" ${test_dependencies} "${C166_LLC}"
        VERBATIM
      )
      list(APPEND objects "${object}")
      math(EXPR index "${index} + 1")
      continue()
    endif()
    set(compile_flags)
    if(extension STREQUAL ".c")
      set(compile_flags
        "-${ARG_OPTIMIZATION}" -ffreestanding -fno-builtin
        -mllvm -verify-machineinstrs -DC166_TEST_LLVM=1 ${defines}
        -I "${C166_TEST_ROOT}/support/include")
      if(source IN_LIST test_sources)
        list(APPEND compile_flags ${ARG_LLVM_COMPILE_OPTIONS})
      endif()
    endif()
    add_custom_command(
      OUTPUT "${object}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}"
      COMMAND "${C166_CLANG}"
        --target=c166-none-elf "-mcmodel=${ARG_MODEL}"
        ${compile_flags}
        -c "${source}" -o "${object}"
      DEPENDS "${source}" ${test_dependencies} "${C166_CLANG}"
      VERBATIM
    )
    list(APPEND objects "${object}")
    math(EXPR index "${index} + 1")
  endforeach()

  set(extra_linker_script_args)
  set(extra_linker_script_dependency)
  if(ARG_LLVM_LINKER_SCRIPT)
    set(extra_linker_script "${ARG_LLVM_LINKER_SCRIPT}")
    if(NOT IS_ABSOLUTE "${extra_linker_script}")
      set(extra_linker_script "${C166_TEST_ROOT}/${extra_linker_script}")
    endif()
    set(extra_linker_script_args -T "${extra_linker_script}")
    set(extra_linker_script_dependency "${extra_linker_script}")
  endif()

  set(elf "${output_dir}/test.elf")
  set(image "${output_dir}/test.hex")
  set(disassembly "${output_dir}/test.dis")
  set(symbols "${output_dir}/test.nm")
  add_custom_command(
    OUTPUT "${elf}"
    COMMAND "${C166_LLD}" -m c166elf
      -T "${linker_script}" ${extra_linker_script_args}
      ${ARG_LLVM_LINK_OPTIONS}
      ${objects} ${test_archive} "${runtime_archive}" -o "${elf}"
    DEPENDS ${objects} "${linker_script}" ${extra_linker_script_dependency}
      ${test_archive} "${runtime_archive}" "${C166_LLD}"
    VERBATIM
  )
  add_custom_command(
    OUTPUT "${image}"
    COMMAND "${C166_OBJCOPY}" -O ihex "${elf}" "${image}"
    DEPENDS "${elf}" "${C166_OBJCOPY}"
    VERBATIM
  )
  add_custom_command(
    OUTPUT "${disassembly}"
    COMMAND "${CMAKE_COMMAND}"
      "-DTOOL=${C166_OBJDUMP}" -DOPTION=-d
      "-DINPUT=${elf}" "-DOUTPUT=${disassembly}"
      -P "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
    DEPENDS "${elf}" "${C166_OBJDUMP}"
    VERBATIM
  )
  add_custom_command(
    OUTPUT "${symbols}"
    COMMAND "${CMAKE_COMMAND}"
      "-DTOOL=${C166_NM}" -DOPTION=-n
      "-DINPUT=${elf}" "-DOUTPUT=${symbols}"
      -P "${C166_TEST_ROOT}/cmake/CaptureToolOutput.cmake"
    DEPENDS "${elf}" "${C166_NM}"
    VERBATIM
  )

  set(build_target
    "c166-${target_component}-${ARG_MODEL}-${ARG_OPTIMIZATION}${platform_component}")
  add_custom_target("${build_target}"
    DEPENDS "${image}" "${disassembly}" "${symbols}")
  add_dependencies("${build_target}" c166-test-runner)
  set_property(GLOBAL APPEND PROPERTY C166_TEST_BUILD_TARGETS "${build_target}")

  set(fixture
    "fixture-${target_component}-${ARG_MODEL}-${ARG_OPTIMIZATION}${platform_component}")
  if(ARG_PLATFORM STREQUAL "c167")
    set(test_prefix "${ARG_MODEL}.${ARG_NAME}")
  else()
    set(test_prefix "${ARG_MODEL}.${ARG_PLATFORM}.${ARG_NAME}")
  endif()
  set(build_test "build.${test_prefix}.llvm.${ARG_OPTIMIZATION}")
  add_test(
    NAME "${build_test}"
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
      --target "${build_target}" --parallel "${C166_TEST_JOBS}")
  set_tests_properties("${build_test}" PROPERTIES
    FIXTURES_SETUP "${fixture}"
    LABELS "build;llvm;${ARG_MODEL};${ARG_PLATFORM}"
    RESOURCE_LOCK c166_cmake_build)

  set(run_test "run.${test_prefix}.llvm.${ARG_OPTIMIZATION}")
  add_test(
    NAME "${run_test}"
    COMMAND c166-test-runner
      --image "${image}"
      --format ihex
      --crossview "${C166_CROSSVIEW}"
      --ini "${C166_CROSSVIEW_INI}"
      --simulator "${simulator_config}"
      --timeout "${ARG_SIMULATOR_TIMEOUT}"
      --work "${output_dir}/run")
  set_tests_properties("${run_test}" PROPERTIES
    FIXTURES_REQUIRED "${fixture}"
    LABELS "simulator;llvm;${ARG_MODEL};${ARG_PLATFORM};runtime"
    TIMEOUT "${ctest_timeout}")

  foreach(kind IN ITEMS NM DIS)
    if(ARG_LLVM_${kind}_CHECK)
      if(kind STREQUAL "NM")
        set(check_input "${symbols}")
      else()
        set(check_input "${disassembly}")
      endif()
      set(check_file "${ARG_LLVM_${kind}_CHECK}")
      if(NOT IS_ABSOLUTE "${check_file}")
        set(check_file "${C166_TEST_ROOT}/${check_file}")
      endif()
      string(TOLOWER "${kind}" check_kind)
      set(check_test
        "static.${test_prefix}.llvm.${ARG_OPTIMIZATION}.${check_kind}")
      add_test(
        NAME "${check_test}"
        COMMAND "${CMAKE_COMMAND}"
          "-DINPUT=${check_input}" "-DPATTERNS=${check_file}"
          -P "${C166_TEST_ROOT}/cmake/CheckPatterns.cmake")
      set_tests_properties("${check_test}" PROPERTIES
        FIXTURES_REQUIRED "${fixture}"
        LABELS "static;llvm;${ARG_MODEL};${ARG_PLATFORM};${check_kind}")
    endif()
  endforeach()

  if(ARG_LLVM_NM_FORBID)
    set(forbid_file "${ARG_LLVM_NM_FORBID}")
    if(NOT IS_ABSOLUTE "${forbid_file}")
      set(forbid_file "${C166_TEST_ROOT}/${forbid_file}")
    endif()
    set(forbid_test
      "static.${test_prefix}.llvm.${ARG_OPTIMIZATION}.nm-forbid")
    add_test(
      NAME "${forbid_test}"
      COMMAND "${CMAKE_COMMAND}"
        "-DINPUT=${symbols}" "-DPATTERNS=${forbid_file}" -DFORBID=ON
        -P "${C166_TEST_ROOT}/cmake/CheckPatterns.cmake")
    set_tests_properties("${forbid_test}" PROPERTIES
      FIXTURES_REQUIRED "${fixture}"
      LABELS "static;llvm;${ARG_MODEL};${ARG_PLATFORM};nm")
  endif()

  if(ARG_LLVM_ONLY)
    return()
  endif()

  set(tasking_dir "${output_dir}/tasking")
  set(tasking_image "${tasking_dir}/test.abs")
  file(MAKE_DIRECTORY "${tasking_dir}")
  _c166_wine_path(tasking_include
    "${C166_TEST_ROOT}/support/include")
  set(tasking_sources)
  set(tasking_dependencies
    "${C166_TEST_ROOT}/support/asc0.c"
    "${C166_TEST_ROOT}/support/tap.c"
    ${test_dependencies}
    ${test_sources})
  foreach(source IN ITEMS
      "${C166_TEST_ROOT}/support/asc0.c"
      "${C166_TEST_ROOT}/support/tap.c")
    _c166_wine_path(tasking_source "${source}")
    list(APPEND tasking_sources "${tasking_source}")
  endforeach()
  if(ARG_CASE_COUNT)
    _c166_wine_path(tasking_source
      "${C166_TEST_ROOT}/support/differential.c")
    list(APPEND tasking_sources "${tasking_source}")
    list(APPEND tasking_dependencies
      "${C166_TEST_ROOT}/support/differential.c")
  endif()
  foreach(source IN LISTS test_sources)
    _c166_wine_path(tasking_source "${source}")
    list(APPEND tasking_sources "${tasking_source}")
  endforeach()
  add_custom_command(
    OUTPUT "${tasking_image}"
    COMMAND "${CMAKE_COMMAND}" -E env
      "WINEPREFIX=${C166_WINEPREFIX}"
      WINEDEBUG=-all
      WINEDLLOVERRIDES=winemenubuilder.exe=d
      wine "${C166_TASKING_CC}"
      "${tasking_model}" ${tasking_arch_flags}
      -x EXTEND "-H${tasking_register_header}" "SN(${tasking_register_file})"
      "-WoSECSIZE(C166_US(+0880H))"
      "-${ARG_OPTIMIZATION}" -DC166_TEST_TASKING=1 ${defines}
      "-I${tasking_include}"
      -g -ieee -o test.abs ${tasking_sources}
    WORKING_DIRECTORY "${tasking_dir}"
    DEPENDS ${tasking_dependencies} ${tasking_toolchain_dependencies}
    VERBATIM
  )
  set(tasking_target
    "c166-${target_component}-${ARG_MODEL}-${ARG_OPTIMIZATION}${platform_component}-tasking")
  add_custom_target("${tasking_target}" DEPENDS "${tasking_image}")
  add_dependencies("${tasking_target}" c166-test-runner)
  set_property(GLOBAL APPEND PROPERTY C166_TEST_BUILD_TARGETS "${tasking_target}")

  set(tasking_fixture
    "fixture-${target_component}-${ARG_MODEL}-${ARG_OPTIMIZATION}${platform_component}-tasking")
  set(tasking_build_test
    "build.${test_prefix}.tasking.${ARG_OPTIMIZATION}")
  add_test(
    NAME "${tasking_build_test}"
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
      --target "${tasking_target}" --parallel "${C166_TEST_JOBS}")
  set_tests_properties("${tasking_build_test}" PROPERTIES
    FIXTURES_SETUP "${tasking_fixture}"
    LABELS "build;tasking;${tasking_architecture};${ARG_MODEL};${ARG_PLATFORM}"
    RESOURCE_LOCK c166_cmake_build)

  set(tasking_run_test
    "run.${test_prefix}.tasking.${ARG_OPTIMIZATION}")
  add_test(
    NAME "${tasking_run_test}"
    COMMAND c166-test-runner
      --image "${tasking_image}"
      --format abs
      --crossview "${C166_CROSSVIEW}"
      --ini "${C166_CROSSVIEW_INI}"
      --simulator "${simulator_config}"
      --timeout "${ARG_SIMULATOR_TIMEOUT}"
      --work "${output_dir}/tasking-run")
  set_tests_properties("${tasking_run_test}" PROPERTIES
    FIXTURES_REQUIRED "${tasking_fixture}"
    LABELS "simulator;tasking;${tasking_architecture};${ARG_MODEL};${ARG_PLATFORM};runtime"
    TIMEOUT "${ctest_timeout}")
endfunction()

function(c166_add_test)
  cmake_parse_arguments(ARG "LLVM_ONLY"
    "NAME;LLVM_LINKER_SCRIPT;LLVM_NM_CHECK;LLVM_NM_FORBID;LLVM_DIS_CHECK;CASE_COUNT;SEED;EXPECTED_SIGNATURE;SIMULATOR_TIMEOUT"
    "SOURCES;LLVM_ARCHIVE_SOURCES;MODELS;PLATFORMS;OPTIMIZATIONS;DEFINITIONS;LLVM_COMPILE_OPTIONS;LLVM_LINK_OPTIONS;LLVM_LINK_OPTIONS_BY_MODEL;LLVM_LINKER_SCRIPT_BY_MODEL;LLVM_NM_CHECK_BY_MODEL;LLVM_NM_FORBID_BY_MODEL;LLVM_DIS_CHECK_BY_MODEL;LLVM_DIS_CHECK_MODELS"
    ${ARGN})
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "c166_add_test(${ARG_NAME}) has unknown arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_NAME OR NOT ARG_SOURCES)
    message(FATAL_ERROR "c166_add_test requires NAME and SOURCES")
  endif()
  if(ARG_LLVM_ARCHIVE_SOURCES AND NOT ARG_LLVM_ONLY)
    message(FATAL_ERROR "LLVM_ARCHIVE_SOURCES requires LLVM_ONLY")
  endif()
  if(NOT ARG_MODELS)
    set(ARG_MODELS large)
  endif()
  if(NOT ARG_OPTIMIZATIONS)
    set(ARG_OPTIMIZATIONS O2)
  endif()
  if(NOT ARG_PLATFORMS)
    set(ARG_PLATFORMS c167)
  endif()
  set(variant_options)
  if(ARG_LLVM_ONLY)
    list(APPEND variant_options LLVM_ONLY)
  endif()

  foreach(model IN LISTS ARG_MODELS)
    _c166_model_values(model_link_options "${model}"
      ${ARG_LLVM_LINK_OPTIONS_BY_MODEL})
    _c166_model_value(extra_linker_script "${model}"
      "${ARG_LLVM_LINKER_SCRIPT}" ${ARG_LLVM_LINKER_SCRIPT_BY_MODEL})
    _c166_model_value(nm_check "${model}" "${ARG_LLVM_NM_CHECK}"
      ${ARG_LLVM_NM_CHECK_BY_MODEL})
    _c166_model_value(nm_forbid "${model}" "${ARG_LLVM_NM_FORBID}"
      ${ARG_LLVM_NM_FORBID_BY_MODEL})
    _c166_model_value(dis_check "${model}" "${ARG_LLVM_DIS_CHECK}"
      ${ARG_LLVM_DIS_CHECK_BY_MODEL})
    if(ARG_LLVM_DIS_CHECK_MODELS AND
       NOT model IN_LIST ARG_LLVM_DIS_CHECK_MODELS)
      set(dis_check "")
    endif()
    foreach(platform IN LISTS ARG_PLATFORMS)
      foreach(optimization IN LISTS ARG_OPTIMIZATIONS)
        _c166_add_test_variant(
          ${variant_options}
          NAME "${ARG_NAME}"
          SOURCES ${ARG_SOURCES}
          LLVM_ARCHIVE_SOURCES ${ARG_LLVM_ARCHIVE_SOURCES}
          DEFINITIONS ${ARG_DEFINITIONS}
          LLVM_COMPILE_OPTIONS ${ARG_LLVM_COMPILE_OPTIONS}
          MODEL "${model}"
          PLATFORM "${platform}"
          OPTIMIZATION "${optimization}"
          LLVM_LINKER_SCRIPT "${extra_linker_script}"
          LLVM_LINK_OPTIONS ${ARG_LLVM_LINK_OPTIONS} ${model_link_options}
          LLVM_NM_CHECK "${nm_check}"
          LLVM_NM_FORBID "${nm_forbid}"
          LLVM_DIS_CHECK "${dis_check}"
          CASE_COUNT "${ARG_CASE_COUNT}"
          SEED "${ARG_SEED}"
          EXPECTED_SIGNATURE "${ARG_EXPECTED_SIGNATURE}"
          SIMULATOR_TIMEOUT "${ARG_SIMULATOR_TIMEOUT}")
      endforeach()
    endforeach()
  endforeach()
endfunction()
