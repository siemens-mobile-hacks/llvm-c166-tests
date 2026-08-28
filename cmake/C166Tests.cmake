function(add_c166_manifest_test manifest)
  get_filename_component(case_dir "${manifest}" DIRECTORY)
  get_filename_component(name "${case_dir}" NAME)
  file(READ "${manifest}" manifest_json)

  string(JSON schema_version GET "${manifest_json}" schema_version)
  if(NOT schema_version EQUAL 2)
    message(FATAL_ERROR "unsupported schema_version in ${manifest}")
  endif()
  string(JSON category GET "${manifest_json}" category)
  string(JSON timeout GET "${manifest_json}" timeout)
  string(JSON optimization_count LENGTH "${manifest_json}" optimizations)
  string(JSON label_count LENGTH "${manifest_json}" labels)
  string(JSON model_count ERROR_VARIABLE model_error
    LENGTH "${manifest_json}" models)
  if(model_error)
    set(model_count 1)
    set(default_model TRUE)
  else()
    set(default_model FALSE)
  endif()
  math(EXPR optimization_last "${optimization_count} - 1")
  math(EXPR label_last "${label_count} - 1")
  math(EXPR model_last "${model_count} - 1")

  # Every ordinary manifest configuration belongs to the optimization matrix.
  # The separate stress runs below deliberately do not inherit this label.
  set(manifest_labels simulator iss optimization)
  if(label_count GREATER 0)
    foreach(index RANGE 0 ${label_last})
      string(JSON label GET "${manifest_json}" labels ${index})
      list(APPEND manifest_labels "${label}")
    endforeach()
  endif()

  foreach(model_index RANGE 0 ${model_last})
    if(default_model)
      set(model large)
    else()
      string(JSON model GET "${manifest_json}" models ${model_index})
    endif()
    if(NOT model STREQUAL "large" AND NOT model STREQUAL "medium" AND
       NOT model STREQUAL "small")
      message(FATAL_ERROR "unsupported model '${model}' in ${manifest}")
    endif()
    foreach(index RANGE 0 ${optimization_last})
      string(JSON optimization GET "${manifest_json}" optimizations ${index})
      set(test_name "sim.${model}.${category}.${name}.${optimization}")
      set(test_labels ${manifest_labels} ${category} ${model})
      if(optimization STREQUAL "O2")
        list(APPEND test_labels smoke)
      endif()
      if(optimization STREQUAL "O0" OR optimization STREQUAL "O2" OR
         optimization STREQUAL "Oz")
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
  if(NOT stress_error)
    math(EXPR stress_last "${stress_count} - 1")
    foreach(stress_index RANGE 0 ${stress_last})
      string(JSON stress_name GET "${manifest_json}"
        stress_runs ${stress_index} name)
      string(JSON stress_model GET "${manifest_json}"
        stress_runs ${stress_index} model)
      string(JSON stress_runner GET "${manifest_json}"
        stress_runs ${stress_index} runner)
      string(JSON stress_timeout GET "${manifest_json}"
        stress_runs ${stress_index} timeout)
      string(JSON stress_arg_count LENGTH "${manifest_json}"
        stress_runs ${stress_index} arguments)
      string(JSON stress_label_count LENGTH "${manifest_json}"
        stress_runs ${stress_index} labels)
      set(stress_args)
      set(stress_labels simulator iss stress ${category} ${stress_model})
      if(stress_arg_count GREATER 0)
        math(EXPR stress_arg_last "${stress_arg_count} - 1")
        foreach(stress_arg_index RANGE 0 ${stress_arg_last})
          string(JSON stress_arg GET "${manifest_json}"
            stress_runs ${stress_index} arguments ${stress_arg_index})
          list(APPEND stress_args "${stress_arg}")
        endforeach()
      endif()
      if(stress_label_count GREATER 0)
        math(EXPR stress_label_last "${stress_label_count} - 1")
        foreach(stress_label_index RANGE 0 ${stress_label_last})
          string(JSON stress_label GET "${manifest_json}"
            stress_runs ${stress_index} labels ${stress_label_index})
          list(APPEND stress_labels "${stress_label}")
        endforeach()
      endif()
      set(stress_test_name
        "sim.${stress_model}.${category}.${name}.${stress_name}")
      add_test(
        NAME "${stress_test_name}"
        COMMAND
          "${C166_TEST_ROOT}/tools/with-isolated-output"
          "${C166_TEST_ROOT}/${stress_runner}"
          ${stress_args}
      )
      set_tests_properties(
        "${stress_test_name}"
        PROPERTIES
          ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
          LABELS "${stress_labels}"
          TIMEOUT "${stress_timeout}"
      )
    endforeach()
  endif()
endfunction()

file(GLOB_RECURSE c166_manifests CONFIGURE_DEPENDS
  "${C166_TEST_ROOT}/tests/*/case.json")
foreach(manifest IN LISTS c166_manifests)
  execute_process(
    COMMAND "${C166_TEST_ROOT}/tools/validate-manifest" "${manifest}"
    RESULT_VARIABLE validation_result
  )
  if(NOT validation_result EQUAL 0)
    message(FATAL_ERROR "invalid test manifest: ${manifest}")
  endif()
  add_c166_manifest_test("${manifest}")
endforeach()

add_test(
  NAME selftest.manifests
  COMMAND
    "${C166_TEST_ROOT}/tools/check-manifests"
    "${C166_TEST_ROOT}"
)
set_tests_properties(
  selftest.manifests
  PROPERTIES LABELS "selftest;infrastructure" TIMEOUT 10
)

add_test(
  NAME selftest.model
  COMMAND "${C166_TEST_ROOT}/selftest/model/test"
)
set_tests_properties(
  selftest.model
  PROPERTIES LABELS "selftest;infrastructure;model" TIMEOUT 10
)

add_test(
  NAME selftest.toolchain
  COMMAND "${C166_TEST_ROOT}/selftest/toolchain/test"
)
set_tests_properties(
  selftest.toolchain
  PROPERTIES LABELS "selftest;infrastructure;toolchain" TIMEOUT 10
)

add_test(
  NAME selftest.session
  COMMAND "${C166_TEST_ROOT}/selftest/session/test"
)
set_tests_properties(
  selftest.session
  PROPERTIES LABELS "selftest;infrastructure;simulator-session" TIMEOUT 10
)

add_test(
  NAME selftest.state
  COMMAND "${C166_TEST_ROOT}/selftest/state/test"
)
set_tests_properties(
  selftest.state
  PROPERTIES LABELS "selftest;infrastructure;run-state" TIMEOUT 10
)

add_test(
  NAME selftest.image
  COMMAND "${C166_TEST_ROOT}/selftest/image/test"
)

add_test(
  NAME selftest.compiler
  COMMAND "${C166_TEST_ROOT}/selftest/compiler/test"
)
set_tests_properties(
  selftest.compiler
  PROPERTIES LABELS "selftest;infrastructure;compiler;orchestration" TIMEOUT 10
)

add_test(
  NAME selftest.stages
  COMMAND "${C166_TEST_ROOT}/selftest/stages/test"
)
set_tests_properties(
  selftest.stages
  PROPERTIES LABELS "selftest;infrastructure;orchestration" TIMEOUT 10
)

add_test(
  NAME selftest.cli
  COMMAND "${C166_TEST_ROOT}/selftest/cli/test"
)
set_tests_properties(
  selftest.cli
  PROPERTIES LABELS "selftest;infrastructure;cli" TIMEOUT 10
)
set_tests_properties(
  selftest.image
  PROPERTIES LABELS "selftest;infrastructure;elf-image" TIMEOUT 10
)

add_test(
  NAME selftest.coverage-report
  COMMAND "${C166_TEST_ROOT}/selftest/coverage-report/test"
)

add_test(
  NAME selftest.control-flow-coverage
  COMMAND "${C166_TEST_ROOT}/selftest/control-flow-coverage/test"
)
set_tests_properties(
  selftest.control-flow-coverage
  PROPERTIES
    ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
    LABELS "selftest;coverage;control-flow;oracle"
    TIMEOUT 30
)
set_tests_properties(
  selftest.coverage-report
  PROPERTIES
    ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
    LABELS "selftest;infrastructure;coverage;simulator;iss"
    RUN_SERIAL TRUE
    TIMEOUT 300
)

add_test(
  NAME selftest.ihex
  COMMAND
    "${CMAKE_COMMAND}"
    "-DTOOL=${C166_TEST_ROOT}/tools/ihex-overlay"
    "-DCASE_DIR=${C166_TEST_ROOT}/selftest/ihex"
    "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/selftest-ihex/actual.hex"
    -P "${C166_TEST_ROOT}/selftest/ihex/test.cmake"
)
set_tests_properties(selftest.ihex PROPERTIES LABELS selftest TIMEOUT 10)

function(add_result_log_selftest name fixture should_fail)
  add_test(
    NAME "selftest.result.${name}"
    COMMAND
      "${C166_TEST_ROOT}/tools/check-result-log"
      block
      "${C166_TEST_ROOT}/selftest/result/${fixture}"
      1
      2459629185
  )
  set_tests_properties(
    "selftest.result.${name}"
    PROPERTIES LABELS "selftest;result-protocol" TIMEOUT 10
  )
  if(should_fail)
    set_tests_properties("selftest.result.${name}" PROPERTIES WILL_FAIL TRUE)
  endif()
endfunction()

add_result_log_selftest(good good.log FALSE)
add_result_log_selftest(bad-status bad-status.log TRUE)
add_result_log_selftest(bad-words bad-words.log TRUE)
add_result_log_selftest(bad-signature bad-signature.log TRUE)
add_result_log_selftest(simulator-error simulator-error.log TRUE)

add_test(
  NAME selftest.symbols.good
  COMMAND
    "${C166_TEST_ROOT}/tools/check-required-symbols"
    "${C166_TEST_ROOT}/selftest/symbols/sample.nm"
    _llvm_entry
    _llvm_mix_helper
)
set_tests_properties(
  selftest.symbols.good PROPERTIES LABELS "selftest;symbols" TIMEOUT 10
)

add_test(
  NAME selftest.symbols.missing
  COMMAND
    "${C166_TEST_ROOT}/tools/check-required-symbols"
    "${C166_TEST_ROOT}/selftest/symbols/sample.nm"
    _missing
)
set_tests_properties(
  selftest.symbols.missing
  PROPERTIES LABELS "selftest;symbols" TIMEOUT 10 WILL_FAIL TRUE
)

add_test(
  NAME selftest.timeout
  COMMAND "${C166_TEST_ROOT}/selftest/timeout/test"
)
set_tests_properties(selftest.timeout PROPERTIES LABELS selftest TIMEOUT 10)

add_test(
  NAME selftest.noninteractive-wine
  COMMAND "${C166_TEST_ROOT}/selftest/noninteractive-wine/test"
)
set_tests_properties(
  selftest.noninteractive-wine
  PROPERTIES LABELS "selftest;wine;noninteractive" TIMEOUT 10
)

add_test(
  NAME selftest.headless-x
  COMMAND "${C166_TEST_ROOT}/selftest/headless-x/test"
)
set_tests_properties(
  selftest.headless-x
  PROPERTIES LABELS "selftest;wine;headless;infrastructure" TIMEOUT 15
)

add_test(
  NAME selftest.vectors
  COMMAND "${C166_TEST_ROOT}/selftest/vectors/test"
)
set_tests_properties(
  selftest.vectors
  PROPERTIES LABELS "selftest;golden-vectors" RUN_SERIAL TRUE TIMEOUT 60
)

add_test(
  NAME selftest.aggregate-failure-reducer
  COMMAND "${C166_TEST_ROOT}/selftest/reducer/test"
)
set_tests_properties(
  selftest.aggregate-failure-reducer
  PROPERTIES LABELS "selftest;infrastructure;reducer" TIMEOUT 10
)

add_test(
  NAME selftest.output-isolation
  COMMAND "${C166_TEST_ROOT}/selftest/output-isolation/test"
)
set_tests_properties(
  selftest.output-isolation
  PROPERTIES LABELS "selftest;output-isolation" TIMEOUT 10
)

add_test(
  NAME selftest.tasking-cstart
  COMMAND
    "${C166_TEST_ROOT}/tools/with-isolated-output"
    "${C166_TEST_ROOT}/tools/check-tasking-cstart"
)
set_tests_properties(
  selftest.tasking-cstart
  PROPERTIES
    ENVIRONMENT "C166_TEST_ROOT=${C166_TEST_ROOT}"
    LABELS "selftest;infrastructure;simulator;iss;startup;tasking;large;medium"
    RUN_SERIAL TRUE
    TIMEOUT 300
)

add_test(
  NAME selftest.fatal-paths
  COMMAND "${C166_TEST_ROOT}/selftest/fatal-paths/test"
)

set_tests_properties(
  selftest.fatal-paths
  PROPERTIES LABELS "selftest;infrastructure;robustness" TIMEOUT 10
)
