# 4. Wrap daily commands in Make

## Context

The supplied brief asks for memorable build, run and test commands. CMake presets
alone work but require longer commands. Shell scripts are another alternative.

## Decision

Propose `make build`, `make run`, `make test` and `make clean`, delegating to CMake
and CTest. `PRESET` selects a Unix preset. Cleaning removes compiled targets and
retains the downloaded dependency and configuration.

## Status

Proposed

## Consequences

The wrapper contains no second build description. Make is required, and native
Windows users use CMake directly. Retaining the cache makes rebuilds quicker but
does not provide a completely fresh configuration.
