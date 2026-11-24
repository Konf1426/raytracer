include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(raytracer_tests
    math/test_vector.cpp
    math/test_cross.cpp
    geometry/test_ray.cpp
    geometry/test_shapes.cpp
    image/test_image.cpp
    render/test_camera.cpp
    render/test_renderer.cpp
)
target_include_directories(raytracer_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(raytracer_tests PRIVATE
    raytracer_core raytracer_warnings Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(raytracer_tests)

add_test(NAME "command line validation and PPM render"
    COMMAND ${CMAKE_COMMAND}
        -DRAYTRACER_EXE=$<TARGET_FILE:raytracer>
        -DTEST_OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cli_test.cmake
)
