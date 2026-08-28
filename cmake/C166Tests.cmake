function(c166_json_array output json)
  string(JSON count LENGTH "${json}" ${ARGN})
  set(values)
  if(count GREATER 0)
    math(EXPR last "${count} - 1")
    foreach(index RANGE 0 ${last})
      string(JSON value GET "${json}" ${ARGN} ${index})
      list(APPEND values "${value}")
    endforeach()
  endif()
  set(${output} "${values}" PARENT_SCOPE)
endfunction()

function(add_c166_manifest_test manifest)
  get_filename_component(case_dir "${manifest}" DIRECTORY)
  get_filename_component(name "${case_dir}" NAME)
  file(READ "${manifest}" manifest_json)

  string(JSON category GET "${manifest_json}" category)
  string(JSON timeout GET "${manifest_json}" timeout)
  c166_json_array(optimizations "${manifest_json}" optimizations)
  c166_json_array(manifest_labels "${manifest_json}" labels)
  string(JSON model_count ERROR_VARIABLE model_error
    LENGTH "${manifest_json}" models)
  if(model_error)
    set(models large)
  else()
    c166_json_array(models "${manifest_json}" models)
  endif()

  list(PREPEND manifest_labels simulator iss optimization)
  foreach(model IN LISTS models)
    foreach(optimization IN LISTS optimizations)
      set(test_name "sim.${model}.${category}.${name}.${optimization}")
      set(test_labels ${manifest_labels} ${category} ${model})
      if(optimization STREQUAL "O2")
        list(APPEND test_labels smoke)
      endif()
      if(optimization MATCHES "^O(0|2|z)$")
        list(APPEND test_labels deterministic)
      endif()
      add_test(
        NAME "${test_name}"
        COMMAND
          "${C166_TEST_ROOT}/tools/with-isolated-output"
          "${C166_TEST_ROOT}/harness/run-sim"
          "${case_dir}"
          "${name}"
          "${optimization}"
          "${model}"
      )
      set_tests_properties(
        "${test_name}"
        PROPERTIES
          ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
          LABELS "${test_labels}"
          TIMEOUT "${timeout}"
      )
    endforeach()
  endforeach()

  string(JSON stress_count ERROR_VARIABLE stress_error
    LENGTH "${manifest_json}" stress_runs)
  if(stress_error)
    return()
  endif()

  math(EXPR stress_last "${stress_count} - 1")
  foreach(index RANGE 0 ${stress_last})
    string(JSON stress_name GET "${manifest_json}" stress_runs ${index} name)
    string(JSON stress_model GET "${manifest_json}" stress_runs ${index} model)
    string(JSON stress_runner GET "${manifest_json}" stress_runs ${index} runner)
    string(JSON stress_timeout GET "${manifest_json}" stress_runs ${index} timeout)
    c166_json_array(stress_args "${manifest_json}" stress_runs ${index} arguments)
    c166_json_array(stress_labels "${manifest_json}" stress_runs ${index} labels)
    list(PREPEND stress_labels simulator iss stress ${category} ${stress_model})
    set(test_name "sim.${stress_model}.${category}.${name}.${stress_name}")
    add_test(
      NAME "${test_name}"
      COMMAND
        "${C166_TEST_ROOT}/tools/with-isolated-output"
        "${C166_TEST_ROOT}/${stress_runner}"
        ${stress_args}
    )
    set_tests_properties(
      "${test_name}"
      PROPERTIES
        ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
        LABELS "${stress_labels}"
        TIMEOUT "${stress_timeout}"
    )
  endforeach()
endfunction()

function(add_c166_selftest name)
  set(options RUN_SERIAL WILL_FAIL)
  set(one_value_args TIMEOUT)
  set(multi_value_args COMMAND ENVIRONMENT LABELS)
  cmake_parse_arguments(TEST
    "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  if(NOT TEST_COMMAND)
    message(FATAL_ERROR "selftest.${name} has no command")
  endif()
  if(NOT TEST_TIMEOUT)
    set(TEST_TIMEOUT 10)
  endif()

  add_test(NAME "selftest.${name}" COMMAND ${TEST_COMMAND})
  set(labels selftest ${TEST_LABELS})
  set_tests_properties("selftest.${name}" PROPERTIES
    LABELS "${labels}" TIMEOUT "${TEST_TIMEOUT}")
  if(TEST_ENVIRONMENT)
    set_tests_properties("selftest.${name}" PROPERTIES
      ENVIRONMENT "${TEST_ENVIRONMENT}")
  endif()
  if(TEST_RUN_SERIAL)
    set_tests_properties("selftest.${name}" PROPERTIES RUN_SERIAL TRUE)
  endif()
  if(TEST_WILL_FAIL)
    set_tests_properties("selftest.${name}" PROPERTIES WILL_FAIL TRUE)
  endif()
endfunction()

file(GLOB_RECURSE c166_manifests CONFIGURE_DEPENDS
  "${C166_TEST_ROOT}/tests/*/case.json")
execute_process(
  COMMAND "${C166_TEST_ROOT}/tools/validate-manifest" ${c166_manifests}
  RESULT_VARIABLE validation_result
)
if(NOT validation_result EQUAL 0)
  message(FATAL_ERROR "invalid test manifest")
endif()
foreach(manifest IN LISTS c166_manifests)
  add_c166_manifest_test("${manifest}")
endforeach()

add_c166_selftest(manifests
  COMMAND "${C166_TEST_ROOT}/tools/check-manifests" "${C166_TEST_ROOT}"
  LABELS infrastructure)
add_c166_selftest(model
  COMMAND "${C166_TEST_ROOT}/selftest/model/test"
  LABELS infrastructure model)
add_c166_selftest(toolchain
  COMMAND "${C166_TEST_ROOT}/selftest/toolchain/test"
  LABELS infrastructure toolchain)
add_c166_selftest(session
  COMMAND "${C166_TEST_ROOT}/selftest/session/test"
  LABELS infrastructure simulator-session)
add_c166_selftest(state
  COMMAND "${C166_TEST_ROOT}/selftest/state/test"
  LABELS infrastructure run-state)
add_c166_selftest(image
  COMMAND "${C166_TEST_ROOT}/selftest/image/test"
  LABELS infrastructure elf-image)
add_c166_selftest(compiler
  COMMAND "${C166_TEST_ROOT}/selftest/compiler/test"
  LABELS infrastructure compiler orchestration)
add_c166_selftest(stages
  COMMAND "${C166_TEST_ROOT}/selftest/stages/test"
  LABELS infrastructure orchestration)
add_c166_selftest(cli
  COMMAND "${C166_TEST_ROOT}/selftest/cli/test"
  LABELS infrastructure cli)
add_c166_selftest(coverage-report
  COMMAND "${C166_TEST_ROOT}/selftest/coverage-report/test"
  ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
  LABELS infrastructure coverage simulator iss
  RUN_SERIAL TIMEOUT 300)
add_c166_selftest(control-flow-coverage
  COMMAND "${C166_TEST_ROOT}/selftest/control-flow-coverage/test"
  ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
  LABELS coverage control-flow oracle
  TIMEOUT 30)
add_c166_selftest(ihex
  COMMAND
    "${CMAKE_COMMAND}"
    "-DTOOL=${C166_TEST_ROOT}/tools/ihex-overlay"
    "-DCASE_DIR=${C166_TEST_ROOT}/selftest/ihex"
    "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/selftest-ihex/actual.hex"
    -P "${C166_TEST_ROOT}/selftest/ihex/test.cmake")

function(add_result_log_selftest name fixture)
  add_c166_selftest("result.${name}"
    COMMAND
      "${C166_TEST_ROOT}/tools/check-result-log"
      block
      "${C166_TEST_ROOT}/selftest/result/${fixture}"
      1
      2459629185
    LABELS result-protocol
    ${ARGN})
endfunction()

add_result_log_selftest(good good.log)
add_result_log_selftest(bad-status bad-status.log WILL_FAIL)
add_result_log_selftest(bad-words bad-words.log WILL_FAIL)
add_result_log_selftest(bad-signature bad-signature.log WILL_FAIL)
add_result_log_selftest(simulator-error simulator-error.log WILL_FAIL)

add_c166_selftest(symbols.good
  COMMAND
    "${C166_TEST_ROOT}/tools/check-required-symbols"
    "${C166_TEST_ROOT}/selftest/symbols/sample.nm"
    _llvm_entry
    _llvm_mix_helper
  LABELS symbols)
add_c166_selftest(symbols.missing
  COMMAND
    "${C166_TEST_ROOT}/tools/check-required-symbols"
    "${C166_TEST_ROOT}/selftest/symbols/sample.nm"
    _missing
  LABELS symbols
  WILL_FAIL)
add_c166_selftest(timeout
  COMMAND "${C166_TEST_ROOT}/selftest/timeout/test")
add_c166_selftest(noninteractive-wine
  COMMAND "${C166_TEST_ROOT}/selftest/noninteractive-wine/test"
  LABELS wine noninteractive)
add_c166_selftest(headless-x
  COMMAND "${C166_TEST_ROOT}/selftest/headless-x/test"
  LABELS wine headless infrastructure
  TIMEOUT 15)
add_c166_selftest(vectors
  COMMAND "${C166_TEST_ROOT}/selftest/vectors/test"
  LABELS golden-vectors
  RUN_SERIAL TIMEOUT 60)
add_c166_selftest(aggregate-failure-reducer
  COMMAND "${C166_TEST_ROOT}/selftest/reducer/test"
  LABELS infrastructure reducer)
add_c166_selftest(output-isolation
  COMMAND "${C166_TEST_ROOT}/selftest/output-isolation/test"
  LABELS output-isolation)
add_c166_selftest(tasking-cstart
  COMMAND
    "${C166_TEST_ROOT}/tools/with-isolated-output"
    "${C166_TEST_ROOT}/tools/check-tasking-cstart"
  ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
  LABELS infrastructure simulator iss startup tasking large medium
  RUN_SERIAL TIMEOUT 300)
add_c166_selftest(fatal-paths
  COMMAND "${C166_TEST_ROOT}/selftest/fatal-paths/test"
  LABELS infrastructure robustness)
