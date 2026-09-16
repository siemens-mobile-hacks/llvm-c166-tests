if(NOT DEFINED TOOL OR NOT DEFINED OPTION OR
   NOT DEFINED INPUT OR NOT DEFINED OUTPUT)
  message(FATAL_ERROR
    "CaptureToolOutput.cmake requires TOOL, OPTION, INPUT, and OUTPUT")
endif()

execute_process(
  COMMAND "${TOOL}" "${OPTION}" "${INPUT}"
  OUTPUT_FILE "${OUTPUT}"
  RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "${TOOL} exited with status ${result}")
endif()
