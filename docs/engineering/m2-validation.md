# m2 implementation and validation

Implementation date: November 24, 2026. This is the observed assistant workflow,
not a claim that the owner personally repeated each exercise. m1 resolves to
commit `91c5735`; its annotated tag is not moved. The m0 discovery report and tag
remain unchanged.

## Test-first observations

| Step | Observed red | Green |
| --- | --- | --- |
| Vector cross | A zero-vector stub failed 2 of 3 new cases: expected +Z and (-3, 6, -3). | Coordinate formula passed all 21 assertions. |
| Triangle | An intersection stub returning no hit failed 6 of 9 cases. | Moller-Trumbore passed all triangle tests; the whole suite passed 45/45. |
| JPEG adapter | A throwing stub failed 4 of 6 cases, including real decode and validation of quality/non-finite channels. | Actual JPEG output passed 32 assertions in 6 cases. |
| CLI integration | A .jpg path still held PPM; the decoder check failed with "Output does not have a JPEG signature". | Main selects JPEG and its default output also decodes at 640 by 360. |

Expected values were not changed to fix these failures. The triangle at
A=(0,0,-5), B=(2,0,-5), C=(0,2,-5), with origin (0.5,0.5,0), direction (0,0,-1),
has t=5, point (0.5,0.5,-5), and unit normal +Z. Doubling direction gives t=2.5.
At (1.5,1.5,-5), barycentric u=v=0.75, outside the triangle. The tilted fixture
has edges (2,0,2), (0,2,2), cross product (-4,-4,4) and t=4.

The JPEG fixtures use sqrt(0.25)*255=127.5, rounded to 128. Uniform grey permits
two levels of lossy error; block-aligned RGB fixtures at quality 100 permit three
for transform/colour-conversion rounding. Dimensions, channels and RGB order are
also checked. A real read is required: signature bytes alone are insufficient.
The Linux /dev/full fixture exercises write failures after successful file open.

## Checks

Docker Desktop's Linux daemon was unavailable. Automatic execution review rejected
a combined download/startup command without a detailed reason. Downloads succeeded
separately; no container or hosted CI success is claimed for m2 here. Native WSL
validation uses the README's isolated build/host-validation tree, never the
container's build/unix-debug cache.

```sh
cmake -S . -B build/host-validation -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host-validation --parallel 2
ctest --test-dir build/host-validation --output-on-failure
cmake --build build/host-validation --target run
./build/host-validation/tests/jpeg_check render.jpg 640 360
```

Debug: 51/51 CTest checks passed; 50 Catch2 cases, 295 assertions passed.
The default 640 by 360 JPEG was decoded and visually inspected: sphere, blue
triangle, floor and shadows are visible. docs/images/m2-preview.jpg is an unchanged
copy of that output.

Release was configured in a separate cache under build/host-validation/release,
reusing only the downloaded Catch2 source, not any build cache:

```sh
cmake -S . -B build/host-validation/release -DCMAKE_BUILD_TYPE=Release \
  -DFETCHCONTENT_SOURCE_DIR_CATCH2="$PWD/build/host-validation/_deps/catch2-src"
cmake --build build/host-validation/release --parallel 2
ctest --test-dir build/host-validation/release --output-on-failure
./build/host-validation/release/raytracer build/host-validation/release/render.jpg 640 360
./build/host-validation/release/tests/jpeg_check build/host-validation/release/render.jpg 640 360
```

Release: 51/51 CTest checks passed. Debug, Release and the checked-in preview
produced identical JPEG SHA-256 values:

```text
f9984a4844fa277a28ec20571861980656c8342c14f3f981e2bbed51f0222071
```

GCC was 13.3.0 and CMake 3.31.10 in native Ubuntu/WSL. No sanitizer or native
Windows/macOS validation of m2 is claimed. git diff --check passed.

## Existing files changed since m1

| File | Reason |
| --- | --- |
| src/math/vector.h, src/math/vector.cpp | Add the cross product required for triangle intersection and its normal. |
| src/main.cpp | Compose the new shape and select JPEG; update help and default path. |
| CMakeLists.txt | Register new source files and the isolated stb encoder target. |
| tests/CMakeLists.txt | Register new test files, decoder target and CLI decoder path. |
| tests/cli_test.cmake | Replace the old PPM-specific output assertion with real JPEG decoding, retaining CLI argument/error checks. |
| .github/workflows/ci.yml | Keep a JPEG artefact and decode it in the existing container job. |
| .gitignore | Ignore the new default generated render.jpg. |
| README.md | Describe current output, commands, tests, dependencies and preview. |
| docs/adr/README.md | Index ADR 0010 without changing any previous status. |
| docs/domain/domain-model.md | Describe Triangle, cross, the JPEG adapter and their contracts. |

All remaining changes are new files. In particular Shape, Hit, Scene, Renderer,
Camera, Sphere, Plane, Image and the PPM adapter keep their m1 source unchanged.
The small byte transfer function is local to each encoder; no working PPM code is
rewritten merely to extract a helper. Third-party code stays behind the adapter.

## Remaining review

Owner review and ADR acceptance, the editor container flow, native Windows/macOS
and hosted CI are separate from these native checks. The m2 checklist records
them without claiming delivery. Historical m1 PNG/Material gaps are explicit
there; this extension does not silently repair or move the m1 snapshot.
