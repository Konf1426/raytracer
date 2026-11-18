# Project instructions

## Commands

Run `make build`, `make run`, `make test` and `make clean` from the repository root
inside the dev container. Prefer the container; if unavailable, use the isolated
`build/host-validation` directory documented in README.md. Never share a CMake
cache between the host and the container.

## Architecture decisions

Read `docs/adr/` before changing the build, tools, tests or architecture. Never go
against an accepted ADR: propose a replacement ADR and ask the owner before
changing that decision. Never set an ADR to `Accepted` yourself.

## How we work

- Write a failing test first; expected values come from the maths, not the code.
- Do not change an expected value to hide a failure; discuss a mistaken expectation.
- Keep changes small and C++ straightforward. Use standard containers and RAII.
- New classes go in the folder for their concept; tests mirror it under `tests/`.
- Includes start from `src/`, for example `#include "math/vector.h"`.
- Write project documentation, identifiers and test names in English.
- Preserve the discovery report and existing milestone tags.
