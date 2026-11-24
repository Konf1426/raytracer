# m2 implementation and validation

Implementation date: November 24, 2026. The initial two-feature extension and the
subsequent Material correction are distinguished below. This is the observed
assistant workflow, not a claim that the owner personally repeated each exercise. m1 resolves to
commit `91c5735`; its annotated tag is not moved. The m0 discovery report and tag
remain unchanged.

## Test-first observations

| Step | Observed red | Green |
| --- | --- | --- |
| Vector cross | A zero-vector stub failed 2 of 3 new cases: expected +Z and (-3, 6, -3). | Coordinate formula passed all 21 assertions. |
| Triangle | An intersection stub returning no hit failed 6 of 9 cases. | Moller-Trumbore passed all triangle tests; the whole suite passed 45/45. |
| JPEG adapter | A throwing stub failed 4 of 6 cases, including real decode and validation of quality/non-finite channels. | Actual JPEG output passed 32 assertions in 6 cases. |
| CLI integration | A .jpg path still held PPM; the decoder check failed with "Output does not have a JPEG signature". | Main selects JPEG and its default output also decodes at 640 by 360. |
| Material | A black-returning stub failed 4 of 7 independent cases, including expected red values 1, 0.8, 0.528 and invalid ambient rejection. | Lambert response and optional ambient/shadow support passed 41 assertions. |
| Shape/material composition | New tests failed to compile: shapes accepted Colour, not Material, and Hit had no material field. | Explicit Material constructors, owned hit values and rendering through all three shapes passed. |

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
separately; the hosted Docker checks are recorded below. Native WSL
validation uses the README's isolated build/host-validation tree, never the
container's build/unix-debug cache.

```sh
cmake -S . -B build/host-validation -DCMAKE_BUILD_TYPE=Debug
cmake --build build/host-validation --parallel 2
ctest --test-dir build/host-validation --output-on-failure
cmake --build build/host-validation --target run
./build/host-validation/tests/jpeg_check render.jpg 640 360
```

Before Material correction: Debug passed 51/51 CTest checks; 50 Catch2 cases,
295 assertions passed. After correction: Debug passed 63/63 CTest checks;
62 Catch2 cases and 373 assertions passed. Existing numerical expectations were
kept: only fixture construction and material colour access changed.
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

Before Material correction, Release passed 51/51 CTest checks. After correction,
Release passed 63/63 CTest checks. Debug, Release and the checked-in preview
produce identical JPEG SHA-256 values, including after the Material correction:

```text
f9984a4844fa277a28ec20571861980656c8342c14f3f981e2bbed51f0222071
```

GCC was 13.3.0 and CMake 3.31.10 in native Ubuntu/WSL. No sanitizer or native
Windows/macOS validation of m2 is claimed. git diff --check passed on project
changes. The unmodified third-party snapshot retains upstream whitespace; the
full m1 comparison uses `git diff --check m1 HEAD -- . ':!third_party'`.

## Material correction during review

The owner requested explicit Material composition after reviewing the m2 wording.
ADR 0011 records this correction of a missing m1 concept. Material owns Colour
and applies Lambert's law. Renderer determines visibility and passes its ambient
setting, then asks the hit's material for the colour. It no longer computes the
Lambert response or scales the surface colour itself.

For N=(0,0,-1) and L=(0.6,0,-0.8), the cosine is 0.8. Orange (1,0.5,0) becomes
(0.8,0.4,0); a grazing or backward-facing surface is black with zero ambient.
For cosine 0.6 and ambient 0.15, brightness is 0.66, so (0.8,0.4,0.2) becomes
(0.528,0.264,0.132). Shadow leaves (0.12,0.06,0.03). These expectations come from
the formula, not observed output. Ownership tests check all shapes and a Hit after
its originating triangle has been destroyed.

After the correction, the default Debug JPEG still has exactly the SHA-256 above,
matching the existing preview byte for byte. Release also passed 63/63 checks and
produced that same JPEG. The original intersection, sunlight and shadow
expectations all remain unchanged. Container validation uses the same workflow
commands and the branch's current CI result.

## Hosted container validation

[GitHub Actions run 45417894682](https://github.com/Konf1426/raytracer/actions/runs/45417894682)
passed both unix-debug and unix-release on commit cfd39a6. Both jobs built the
repository's Dockerfile, ran make test inside that image, rendered a JPEG and
decoded it before uploading the image artifact. This validates the actual
container build, not the editor's Dev Containers reopening flow.

The follow-up workflow also runs make run, decodes the default render.jpg and
runs make clean after validation, keeping the JPEG artifact. See the branch's
latest workflow for the result of those added command checks.

[Run 45418537718](https://github.com/Konf1426/raytracer/actions/runs/45418537718)
passed those expanded commands in both configurations before the Material
correction. The current correction is validated separately on the
[implementation branch's CI](https://github.com/Konf1426/raytracer/actions?query=branch%3Am2-triangle-jpeg).

## Existing files changed since m1

| File | Reason |
| --- | --- |
| src/math/vector.h, src/math/vector.cpp | Add the cross product required for triangle intersection and its normal. |
| src/main.cpp | Compose the new shape and select JPEG; update help and default path. |
| CMakeLists.txt | Register new source files, Material and the isolated stb encoder target. |
| tests/CMakeLists.txt | Register new test files, decoder target and CLI decoder path. |
| tests/cli_test.cmake | Replace the old PPM-specific output assertion with real JPEG decoding, retaining CLI argument/error checks. |
| .github/workflows/ci.yml | Keep a JPEG artefact and decode it in the existing container job. |
| .gitignore | Ignore the new default generated render.jpg. |
| README.md | Describe current output, commands, tests, dependencies and preview. |
| docs/adr/README.md | Index ADRs 0010 and 0011 without changing any previous status. |
| docs/domain/domain-model.md | Describe Triangle, cross, the JPEG adapter and their contracts. |
| src/geometry/hit.h | Replace the surface Colour with a Material value so hits carry shading behaviour without borrowed lifetimes. This corrects the missing m1 concept. |
| src/geometry/sphere.h, src/geometry/sphere.cpp | Compose Sphere with Material and return it in Hit; leave all intersection arithmetic unchanged. |
| src/geometry/plane.h, src/geometry/plane.cpp | Apply the same material contract to Plane; leave all intersection arithmetic unchanged. |
| src/render/renderer.cpp | Delegate the surface response to Material while keeping the existing visibility query and ambient configuration. |
| tests/geometry/test_shapes.cpp, tests/render/test_renderer.cpp | Construct Material explicitly and inspect its colour in existing fixtures. Keep every expected t, point, normal and rendered colour unchanged. |

All remaining changes are new files. In particular Shape, Scene, Camera, Image
and the PPM adapter keep their m1 source unchanged. The extra Hit, Sphere, Plane
and Renderer changes correct the missing m1 material composition; they are not
attributed to adding triangle geometry or JPEG output.
The small byte transfer function is local to each encoder; no working PPM code is
rewritten merely to extract a helper. Third-party code stays behind the adapter.

## Remaining review

Owner review and ADR acceptance, the editor container flow and native Windows/macOS
are separate from these checks. The m2 checklist records
them without claiming delivery. Historical m1 gaps are explicit there: the
Material gap is corrected in m2, while the m1 snapshot and its PPM output stay
frozen. The JPEG library ADR and Material ADR remain Proposed for owner review.
