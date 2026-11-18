# 1. Use a development container

## Context

The supplied project brief asks for the same C++ environment on different laptops.
Native tool installation is an alternative, but versions and setup steps differ.

## Decision

Propose a Linux dev container based on the course's Ubuntu 24.04 C++ image. Keep
the Dockerfile and editor configuration in `.devcontainer/`. Use the build tools
already supplied by the image and verify their presence during the build. The
editor uses CMake Tools and Microsoft's C/C++ extension.

## Status

Proposed

## Consequences

Contributors share an environment, including CI. Docker needs disk space and must
be running; Windows users need WSL integration. The image tag is not an immutable
lock. Avoiding redundant apt installation removes a network dependency that
failed during validation. Additional tools would need an explicit installation
step. CPU architecture still varies, and native Windows/macOS builds need
separate validation.
