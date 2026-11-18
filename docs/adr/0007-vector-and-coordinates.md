# 7. Use value vectors and explicit invalid-direction errors

## Context

The course supplies vector arithmetic in 3D. Points and directions use the same
arithmetic today. Normalising a zero vector becomes relevant when generating rays
and defining surface normals; silently producing NaN would corrupt intersections.

## Decision

Propose a `Vector` value type with private coordinates, getters and operations
returning new values. Use it for both positions and directions, with double
precision. Keep colour separate because RGB channels are not spatial axes.
`normalised()` rejects zero or non-finite length with `std::invalid_argument`.
Rays reject invalid direction length but preserve nonunit directions.

## Status

Proposed

## Consequences

The ray equation reads `origin + t * direction`. There are no coordinate setters;
an assignable variable can still be replaced by another value. Separate Point and
Direction types would prevent some meaningless expressions but add complexity.
Callers must handle invalid input. Double arithmetic has finite range and rounding
error; tests compare with a tolerance. With nonunit directions, t is a parameter,
not a world-space distance. Extreme coordinate scales are outside the m1 scope.
