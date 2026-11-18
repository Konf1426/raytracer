# 6. Organise code by domain concept

## Context

The brief asks for a maintainable project with matching source and test folders.
A flat source directory becomes harder to navigate as geometry and rendering grow.

## Decision

Propose `math/`, `geometry/`, `image/` and `render/` under `src/`, mirrored under
`tests/`. Includes use paths relative to `src/`. Keep one core library and a small
command-line entry point.

## Status

Proposed

## Consequences

A class's concept identifies where its tests live. Moves require CMake and
include updates. Folders alone do not enforce dependency boundaries. Defer
namespaces until integration risks name collisions, separate libraries until
dependency enforcement or build times justify them, and a public `include/`
directory until the project distributes a library.
