# 8. Render matte shapes with a directional sun

## Context

The m1 objective is a sphere on a floor, lit by the sun. The detailed setup and
Vector tutorials are published, but the remaining implementation is an engineering
proposal rather than a claim about unpublished course exercises.

## Decision

Propose a small `Shape` interface implemented by `Sphere` and infinite `Plane`.
An intersection returns the nearest Hit in the open interval `(t_min, t_max)`, or
no value. Normals point out of spheres and along the plane's configured normal.
Scene owns shapes through unique pointers; the renderer uses Scene's hit query.

Use a pinhole camera pointing along -Z, with +Y up. Pixel (0, 0) is top left and
one primary ray goes through each pixel centre. White sunlight uses one normalised
direction towards the sun, Lambert cosine shading, ambient light of 0.15 and a
shadow ray. Offset shadow origins and exclude hits up to `ray_epsilon = 1e-6` to
avoid self-intersections. Keep rendering deterministic and single-threaded.

## Status

Proposed

## Consequences

A new shape can implement the same contract without changing the render loop.
Virtual dispatch and dynamic ownership add some overhead but remain small here.
Hard shadows and a constant sky are easy to explain and test. Ambient light is an
artistic approximation, not global illumination. A fixed epsilon assumes the demo's
world-unit scale. Defer camera rotation, antialiasing, reflections, refraction,
acceleration structures and parallelism until a concrete requirement justifies
them. One sample per pixel produces visibly stepped edges.
