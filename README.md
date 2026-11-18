# Raytracer

A C++20 student ray tracer for HETIC, developed from Kevin Glass's
[course](https://learn.glassworks.tech/raytracer/).

The m1 implementation renders a matte sphere touching a floor, with directional
sunlight, hard shadows and a sky background. The image below is produced by this
program, converted losslessly from PPM to PNG for browser display.

![m1: a sunlit red sphere on a floor](docs/images/m1-preview.png)

## Development environment

Install Docker and VS Code's Dev Containers extension. On Windows, keep the
repository inside WSL and enable that distribution in Docker Desktop's WSL
integration settings. Open the repository and select **Dev Containers: Reopen in
Container**. Docker must be running. The first configure downloads Catch2 from
GitHub and requires internet access.

Inside the container, from the repository root:

```sh
make build
make run
make test
make clean
```

`make run` creates `render.ppm` (640 × 360) in the repository root. Open it with a
PPM-compatible image viewer. Generated files and build caches are ignored by Git.
`make clean` removes compiled targets but keeps the CMake cache and downloaded
Catch2, so subsequent builds need not download it again.

To choose a path and resolution after building:

```sh
./build/unix-debug/raytracer build/custom.ppm 1280 720
./build/unix-debug/raytracer --help
make test PRESET=unix-release
```

The CLI accepts `raytracer [output.ppm [width height]]`; both dimensions must be
integers from 1 to 4096. Output folders must already exist. Invalid arguments and
file errors return a nonzero exit status. File contents are always P3 PPM,
regardless of the chosen extension.

CMake describes the build, Catch2 provides C++ assertions, and CTest discovers
and runs the tests. The Makefile wraps those tools in short commands. No external
library is required at runtime.

Without Docker, install a C++20 compiler, Git, Make and CMake 3.25 or newer.
Use a separate build directory so that the container never reuses a host cache:

```sh
cmake -S . -B build/host-validation -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host-validation --parallel 2
ctest --test-dir build/host-validation --output-on-failure
./build/host-validation/raytracer
```

On native Windows, install Visual Studio 2022 with **Desktop development with
C++**, CMake 3.25+ and Git, then use the presets directly:

```powershell
cmake --preset windows
cmake --build --preset windows-debug
ctest --preset windows-debug
.\build\windows\Debug\raytracer.exe render.ppm 640 360
```

Native Windows/macOS builds are provided as configurations but have not been
validated. The supported shared development environment is the Linux container.

## Tests and decisions

Use TDD: write a test with expected values calculated independently, observe the
failure, implement the smallest change, then refactor with the tests green.
Add each new test file explicitly to `tests/CMakeLists.txt`.

There are 32 Catch2 cases plus one CLI integration test. They cover vector maths,
ray parameters, sphere/plane intersections, closest-hit selection, camera pixel
centres, image bounds, PPM encoding, sunlight, shadows and command-line errors.
See the [TDD record](docs/engineering/tdd-log.md) and
[engineering review](docs/engineering/review.md). The
[validation record](docs/engineering/validation.md) includes successful Debug and
Release checks in WSL and in a fresh copy inside the Docker image.

Architecture decisions are recorded in [docs/adr](docs/adr/README.md). Drafts stay
`Proposed` until the project owner reads and accepts them. AI instructions live in
[AGENTS.md](AGENTS.md). The original [m0 report](docs/discovery/discovery-report.md)
is preserved.

## Project map

```text
src/
  main.cpp       command-line parsing and demonstration scene
  math/          3D vectors
  geometry/      rays, hits and shapes (sphere and plane)
  image/         linear RGB pixels and separate PPM encoding
  render/        camera, scene ownership and sunlight rendering
tests/           mirrors the source domains, plus CLI checks
docs/
  adr/           proposed architecture decisions
  domain/        UML model and rendering contracts
  engineering/   TDD observations and code review
  milestones/    m1 delivery checklist
```

The [domain model](docs/domain/domain-model.md) explains the coordinates and maths.
The camera faces -Z with +Y up; one ray passes through each pixel centre. Shadows
use an extra ray towards the sun. Shading is Lambertian with a small ambient term;
the PPM writer clamps and gamma encodes the final linear colour.

The scope is deliberately small: fixed camera orientation, one sample per pixel,
no reflections, indirect illumination, acceleration structure or parallelism.
The intersection epsilon assumes the demo's world scale. This is the m1 proposal,
not a production renderer or an implementation of unpublished course details.

## Contributing and delivery

Read [CONTRIBUTING.md](CONTRIBUTING.md) and the
[code of conduct](CODE_OF_CONDUCT.md). Issue and pull request templates live under
`.github/`. [CI](.github/workflows/ci.yml) builds the development Dockerfile and
runs tests and a small render in Debug and Release. A successful local run does
not establish that a hosted workflow has passed.

The [m1 checklist](docs/milestones/m1.md) separates the implementation from final
review, ADR acceptance and Git delivery. The project owner must be able to explain
and defend the decisions before tagging the milestone.

Licensed under [MIT](LICENSE), as selected by the project owner. Dependencies
retain their own licences.
