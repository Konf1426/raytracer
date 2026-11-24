include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_library(stb_read STATIC support/stb_read.cpp)
target_include_directories(stb_read SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/third_party/stb)

add_executable(raytracer_tests
    math/test_vector.cpp
    math/test_cross.cpp
    geometry/test_ray.cpp
    geometry/test_shapes.cpp
    geometry/test_triangle.cpp
    geometry/test_material_composition.cpp
    image/test_image.cpp
    image/test_jpeg.cpp
    material/test_material.cpp
    render/test_camera.cpp
    render/test_renderer.cpp
)
target_include_directories(raytracer_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(raytracer_tests PRIVATE
    raytracer_core raytracer_warnings Catch2::Catch2WithMain stb_read)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(raytracer_tests)

add_executable(jpeg_check support/jpeg_check.cpp)
target_include_directories(jpeg_check PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(jpeg_check PRIVATE stb_read raytracer_warnings)

add_test(NAME "command line validation and JPEG render"
    COMMAND ${CMAKE_COMMAND}
        -DRAYTRACER_EXE=$<TARGET_FILE:raytracer>
        -DJPEG_CHECK_EXE=$<TARGET_FILE:jpeg_check>
        -DTEST_OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cli_test.cmake
)
