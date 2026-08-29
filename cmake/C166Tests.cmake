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
  string(JSON variant_count ERROR_VARIABLE variant_error
    LENGTH "${manifest_json}" tasking_variants)
  if(variant_error)
    set(runtime_variants ext)
  else()
    c166_json_array(runtime_variants "${manifest_json}" tasking_variants)
  endif()
  string(JSON model_count ERROR_VARIABLE model_error
    LENGTH "${manifest_json}" models)
  if(model_error)
    set(models large)
  else()
    c166_json_array(models "${manifest_json}" models)
  endif()

  list(PREPEND manifest_labels simulator iss optimization)
  foreach(model IN LISTS models)
    foreach(runtime_variant IN LISTS runtime_variants)
      foreach(optimization IN LISTS optimizations)
        set(variant_component "")
        if(NOT runtime_variant STREQUAL "ext")
          set(variant_component ".${runtime_variant}")
        endif()
        set(test_name
          "sim.${model}${variant_component}.${category}.${name}.${optimization}")
        set(test_labels
          ${manifest_labels} ${category} ${model} ${runtime_variant})
        if(optimization STREQUAL "O2")
          list(APPEND test_labels smoke)
        endif()
        if(optimization MATCHES "^O(0|2|z)$")
          list(APPEND test_labels deterministic)
        endif()
        add_test(
          NAME "${test_name}"
          COMMAND
            "${C166_TEST_ROOT}/harness/run-sim"
            "${case_dir}"
            "${name}"
            "${optimization}"
            "${model}"
            "${runtime_variant}"
        )
        set_tests_properties("${test_name}" PROPERTIES
          LABELS "${test_labels}" TIMEOUT "${timeout}")
      endforeach()
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
        "${C166_TEST_ROOT}/${stress_runner}"
        ${stress_args}
    )
    set_tests_properties("${test_name}" PROPERTIES
      LABELS "${stress_labels}" TIMEOUT "${stress_timeout}")
  endforeach()
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
