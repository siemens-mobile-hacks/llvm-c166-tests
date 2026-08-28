if(NOT DEFINED TOOL OR NOT DEFINED CASE_DIR OR NOT DEFINED OUTPUT)
  message(FATAL_ERROR "TOOL, CASE_DIR and OUTPUT are required")
endif()

get_filename_component(output_dir "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${output_dir}")

execute_process(
  COMMAND
    "${TOOL}" --replace 0x11:0x12
    "${CASE_DIR}/base.hex" "${CASE_DIR}/overlay.hex" -o "${OUTPUT}"
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "ihex-overlay failed (${result}):\n${stdout}${stderr}")
endif()

execute_process(
  COMMAND "${CMAKE_COMMAND}" -E compare_files "${CASE_DIR}/expected.hex" "${OUTPUT}"
  RESULT_VARIABLE result
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "ihex-overlay output differs from expected.hex")
endif()
