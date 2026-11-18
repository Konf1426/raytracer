# Validation record

Checked on November 18, 2026, on x86-64. These results apply to the local working
tree prepared during this session; no hosted CI result is claimed.

| Environment | Check | Result |
| --- | --- | --- |
| Native Ubuntu in WSL, GCC 13.3.0, CMake 3.31.10 | Debug build, unit tests and CLI integration | 33/33 passed |
| Native Ubuntu in WSL, GCC 13.3.0, CMake 3.31.10 | Release build, unit tests and CLI integration | 33/33 passed |
| Development Docker image, GCC 13.3.0, CMake 3.28.3 | Fresh source copy, Debug, as vscode | 33/33 passed; default render produced |
| Same Docker source copy and user | Release build and tests | 33/33 passed; default-size render produced |
| WSL GCC, address/undefined sanitizers, leak detection | Project code and unit tests compiled with instrumentation | 32 cases, 180 assertions passed; no diagnostic |
| PPM output | Header, channel range, pixel count and visual inspection | 640 × 360, 230400 RGB pixels; sphere, floor and shadow visible |
| Repository configuration | JSON, CI YAML, local documentation links and line endings | Checked |
| Existing m0 work | Discovery report diff and tag list | Report unchanged; m0 preserved |

The Docker image was built from `.devcontainer/Dockerfile`. A source archive
included tracked files and unignored additions but excluded `.git`, host caches,
local presets and generated renders. The archive was extracted into a new
container, owned by `vscode`, before running:

```sh
make build
make test
make run
make test PRESET=unix-release
./build/unix-release/raytracer build/unix-release/render.ppm 640 360
```

The default PPM from native WSL and both Docker configurations had this SHA-256:

```text
67ec98014f0a13c00c3a74bd32b5337e6f01a6b73d27c44c48edb5d6a6695ae4
```

This demonstrates consistency for these environments and this scene, not a
cross-platform guarantee. The README preview is a lossless conversion of those
RGB bytes to PNG. Rendering has no randomness.

## Limits and remaining delivery work

The editor's Dev Containers reopening flow, a classmate's machine and native
Windows/macOS were not exercised. GitHub Actions must run after reviewed changes
are pushed. The owner still needs to review the implementation, accept or revise
the proposed ADRs, and create the m1 tag on the intended commit.

Development logs are local under ignored `build/`, including
`container-validation.log`, `sanitizers.log`, `release-*.log` and `tdd-*.log`.
The temporary native CMake installation was outside the repository and did not
change system packages. Normal development uses CMake supplied by the container.
