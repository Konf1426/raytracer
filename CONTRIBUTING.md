# Contributing

Read the [README](README.md), [code of conduct](CODE_OF_CONDUCT.md) and
[architecture decisions](docs/adr/README.md) first. This project is licensed under
the [MIT licence](LICENSE); Catch2 and the container image retain their own
licences.

## Environment

Open the repository in its dev container with Docker running. Inside its terminal:

```sh
make build
make test
make run
```

The first configure downloads Catch2. Do not reuse a CMake cache created on a
different machine or outside the container. README documents an isolated native
build when Docker is unavailable.

## A change

1. Describe the problem and its expected behaviour in an issue or pull request.
2. Create a branch for a focused change.
3. Write a failing test first. Derive expected values from geometry or an
   independent example, then implement and refactor with tests green.
4. Put source files in their domain folder and tests in the matching folder under
   `tests/`. Add new cpp files explicitly to the CMake lists.
5. Run `make test`, update affected documentation and describe your validation.
6. Submit a pull request using the template; CI builds the same Dockerfile and
   runs the tests and a small render in Debug and Release.

Use the checked-in `.clang-format` style. Prefer small functions, explicit
ownership, standard containers and readable domain names.

## Decisions

Before changing architecture or tools, read `docs/adr/`. Explain alternatives and
costs in a new numbered ADR with status `Proposed`. An accepted ADR is superseded
by a new record, not silently rewritten. Only the owner accepts decisions. Do not
mark an AI-generated record accepted on the owner's behalf.

## Reporting problems

Include the OS/architecture, whether the dev container was used, compiler and
CMake versions, the exact command and the relevant error output. Never post
credentials or private personal information in an issue.
