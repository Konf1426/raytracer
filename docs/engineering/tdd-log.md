# TDD observations

Implementation session: November 18, 2026. The starting repository held the m0
report and a minimal README, with an existing m0 tag. This is a record of observed
checks, not a claim that the student personally completed every interactive course
prompt or made a commit at every step.

The [Vector tutorial](https://learn.glassworks.tech/raytracer/the-vector-class/step-by-step)
provided the arithmetic examples. Expected values below were taken from those
examples or calculated from the stated geometry, never from the implementation.

| Step | Observed red | Green or refactor |
| --- | --- | --- |
| Bootstrap | Not a TDD exercise | Hello World printed; both greeting tests passed. |
| Vector declarations and addition test | Linker reported undefined Vector constructor, getters and add. | Added definitions. |
| Deliberately wrong addition | Stub returned zero; assertion showed `0.0 == 7`. | Implemented coordinate sums; three tests passed including greetings. |
| Nonzero z | Replacing the z sum with zero still passed the 2D example. A new 3D test failed with `0.0 == 3`. | Restored the z sum. |
| `operator+` | Compiler reported no matching operator. | Added operator, then removed add and the duplicate test. |
| Subtraction | New tests failed to compile before the operator existed. | Checked (3, 4, 0) and (3, 4, 4) results. |
| Scalar-left multiplication | New tests failed to compile before the free function existed. | Checked positive, zero and negative scalars. |
| Dot and length | New tests failed to compile before their methods existed. | Checked 24, 0 and 36 dot products, and lengths 5 and 13. Length reuses dot. |
| Normalisation | Missing method first; then exact comparison failed with `0.60000000000000009 == 0.59999999999999998`. | Kept the arithmetic and expected values; changed the comparison to absolute tolerance 1e-9. |
| Zero normalisation | Exception assertion failed because no exception was thrown. | Reject zero or non-finite magnitude explicitly. |
| Geometry and rendering | Tests were written first and failed to compile because domain headers did not exist. | Implemented the tested contracts; all 34 cases, including two greeting cases, passed. |
| CLI | Integration test failed because `--help` still printed Hello World. | Added rendering and argument validation; removed the obsolete greeting code and tests. |

The final suite has 32 Catch2 cases and one CTest CLI integration case. Logs from
the development runs are local under ignored `build/`; the tests themselves are
the lasting executable specification.

## Independent rendering expectations

- Sphere centre (0, 0, -5), radius 1, origin zero, direction (0, 0, -1): roots
  4 and 6. Scaling the direction by two gives t = 2 for the same first hit.
- Floor y = 0, origin (0, 8, 0), direction (3, -4, 0): t = 2, point (6, 0, 0).
- A unit upward normal and upward sun give full surface colour. A perpendicular
  sun leaves only 15% ambient. Sun (0, 1, 1) gives `0.15 + 0.85 / sqrt(2)`.
- A sphere above the sampled floor point blocks an upward shadow ray but does
  not obstruct the primary ray travelling down from below that sphere.
- Linear channel 0.25 becomes sqrt(0.25) × 255, rounded to 128 in the PPM writer.

The owner should now read the implementation, redo selected calculations and
review the proposed ADRs. A passing suite only establishes its checked cases.
