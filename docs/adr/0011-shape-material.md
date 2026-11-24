# 11. A shape owns a material and delegates its surface response

## Context

The [m2 request](https://learn.glassworks.tech/raytracer/milestone-m2/) specifies
three corners and a material. The published
[m1 lesson](https://learn.glassworks.tech/raytracer/shading-and-shapes/step-by-step)
puts Lambert's law in Material and composes shapes with that value. Our frozen
m1 instead stores Colour on each shape and computes the response in Renderer.
The owner requested this correction after the first m2 implementation review.

## Options

- Keep Colour on shapes: the existing picture works, but geometry and shading
  still omit the explicit material behaviour requested by the course.
- Derive MatteSphere, MattePlane and MatteTriangle: mixes geometry with surface
  response and multiplies classes when another material is introduced.
- Give each shape a Material value: separates the two concepts and lets every
  shape reuse one surface response without shared ownership or lifetime rules.

## Decision

Propose a concrete Material value in material/, with matching tests. It owns a
Colour and exposes colour() and shade(normal, to_light). For unit vectors,
the two-argument form implements the course's max(0, N dot L) times colour.
An optional ambient fraction (default zero) and shadow flag (default false)
preserve the existing renderer's ambient light and hard shadows. The response is
`(ambient + (1 - ambient) * direct) * colour`, where direct is zero in shadow,
otherwise max(0, N dot L). Ambient must be finite and between zero and one.

Sphere, Plane and Triangle take and own a Material instead of Colour. Construction
from Colour is explicit, so callers state the composition. Hit carries a copy of
the material, just as it previously carried a copy of Colour: a returned hit never
borrows from a destroyed or moved shape. Renderer still determines visibility and
keeps its existing sun/ambient configuration API, then delegates the colour
calculation to the hit's Material. Scene and the Shape intersection interface do
not change. No speculative material interface is needed for one surface model.

## Status

Proposed

## Consequences

This adds behaviour, not just a renamed colour wrapper. Independent Lambert
examples verify the response. Existing geometry and renderer tests keep their
numeric expectations, changing only how fixtures construct and inspect materials.
The demonstration should produce exactly the same JPEG before and after this
correction, including its ambient term and shadow visibility.

Hit and the existing Sphere/Plane constructors change, as does Renderer internally.
These additional changes from m1 are an explicit correction of the missing m1
composition, not necessary changes to add a triangle to an already-correct m1.
The validation record lists and justifies them. This refines proposal 0008;
no accepted ADR is overridden. Old records and milestone tags stay unchanged.
The historical m1 PPM/PNG discrepancy cannot be repaired by changing its tag.
