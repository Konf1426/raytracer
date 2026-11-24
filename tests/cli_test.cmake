if(NOT DEFINED RAYTRACER_EXE OR NOT DEFINED TEST_OUTPUT_DIR OR NOT DEFINED JPEG_CHECK_EXE)
    message(FATAL_ERROR "Missing CLI test configuration")
endif()

execute_process(COMMAND "${RAYTRACER_EXE}" --help
    RESULT_VARIABLE result OUTPUT_VARIABLE help ERROR_VARIABLE error)
if(NOT result EQUAL 0 OR NOT help MATCHES "Usage:")
    message(FATAL_ERROR "--help failed: ${help}${error}")
endif()

foreach(width IN ITEMS 0 -1 abc 12px 4097 9999999999999999999999999)
    execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.jpg" "${width}" 4
        RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
    if(result EQUAL 0)
        message(FATAL_ERROR "Invalid width '${width}' was accepted")
    endif()
endforeach()

execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.jpg" 8
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
if(result EQUAL 0)
    message(FATAL_ERROR "An incomplete resolution was accepted")
endif()

execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/missing-directory/render.jpg" 8 4
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
if(result EQUAL 0)
    message(FATAL_ERROR "An output error was ignored")
endif()

file(REMOVE "${TEST_OUTPUT_DIR}/cli.jpg")
execute_process(COMMAND "${RAYTRACER_EXE}" "${TEST_OUTPUT_DIR}/cli.jpg" 8 4
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Render failed: ${output}${error}")
endif()
execute_process(COMMAND "${JPEG_CHECK_EXE}" "${TEST_OUTPUT_DIR}/cli.jpg" 8 4
    RESULT_VARIABLE result ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Invalid CLI JPEG: ${error}")
endif()

file(REMOVE "${TEST_OUTPUT_DIR}/render.jpg")
execute_process(COMMAND "${RAYTRACER_EXE}"
    WORKING_DIRECTORY "${TEST_OUTPUT_DIR}"
    RESULT_VARIABLE result OUTPUT_QUIET ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Default render failed: ${error}")
endif()
execute_process(COMMAND "${JPEG_CHECK_EXE}" "${TEST_OUTPUT_DIR}/render.jpg" 640 360
    RESULT_VARIABLE result ERROR_VARIABLE error)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Invalid default JPEG: ${error}")
endif()
