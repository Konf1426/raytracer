cmake_minimum_required(VERSION 3.25)
project(raytracer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(raytracer_warnings INTERFACE)
if(MSVC)
    target_compile_options(raytracer_warnings INTERFACE /W4)
else()
    target_compile_options(raytracer_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

add_library(raytracer_core
    src/math/vector.cpp
    src/geometry/ray.cpp
    src/geometry/sphere.cpp
    src/geometry/plane.cpp
    src/geometry/triangle.cpp
    src/image/colour.cpp
    src/image/image.cpp
    src/image/ppm.cpp
    src/render/camera.cpp
    src/render/scene.cpp
    src/render/renderer.cpp
)
target_include_directories(raytracer_core PUBLIC src)
target_link_libraries(raytracer_core PRIVATE raytracer_warnings)

add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer PRIVATE raytracer_core raytracer_warnings)
add_custom_target(run
    COMMAND $<TARGET_FILE:raytracer>
    DEPENDS raytracer
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    USES_TERMINAL
)

option(RAYTRACER_BUILD_TESTS "Build the unit tests" ON)
if(RAYTRACER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
