# 2. Describe builds with CMake and run tests with CTest

## Context

The brief asks for a cross-platform C++ build. Handwritten platform build files
would duplicate configuration; Meson is another possible build system.

## Decision

Propose CMake 3.25+, C++20 and versioned debug/release presets. Keep application
logic in `raytracer_core`, shared by the executable and tests. Apply warnings only
to our targets. CTest runs individually discovered Catch2 cases.

## Status

Proposed

## Consequences

One build description supports multiple generators. CMake is an additional
language to learn, and each host/container needs its own cache. Presets describe
native builds but do not prove portability. CTest runs tests; it does not compile.
