if(NOT DEFINED RAYTRACER_EXE OR NOT DEFINED TEST_OUTPUT_DIR)
    message(FATAL_ERROR "Missing CLI test configuration")
endif()

execute_process(COMMAND "${RAYTRACER_EXE}" --help
    RESULT_VARIABLE result OUTPUT_VARIABLE help ERROR_VARIABLE error)
if(NOT result EQUAL 0 OR NOT help MATCHES "Usage:")
    message(FATAL_ERROR "--help failed: ${help}${error}")
endif()

foreach(width IN ITEMS 0 -1 abc 12px 4097 9999999999999999999999999)
    execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.ppm" "${width}" 4
        RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
    if(result EQUAL 0)
        message(FATAL_ERROR "Invalid width '${width}' was accepted")
    endif()
endforeach()

execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.ppm" 8
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
if(result EQUAL 0)
    message(FATAL_ERROR "An incomplete resolution was accepted")
endif()

execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/missing-directory/render.ppm" 8 4
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
if(result EQUAL 0)
    message(FATAL_ERROR "An output error was ignored")
endif()

execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.ppm" 8 4
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Render failed: ${output}${error}")
endif()
file(READ "${TEST_OUTPUT_DIR}/cli.ppm" ppm)
if(NOT ppm MATCHES "^P3\n8 4\n255\n")
    message(FATAL_ERROR "Incorrect PPM header")
endif()
string(REGEX MATCHALL "[0-9]+" values "${ppm}")
list(LENGTH values count)
if(NOT count EQUAL 100)
    message(FATAL_ERROR "Expected 32 RGB pixels and the PPM header, found ${count} numbers")
endif()
