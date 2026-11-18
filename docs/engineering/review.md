# Engineering review

This review describes the implemented m1 proposal. It does not substitute for
the owner's review or accept the proposed ADRs.

| Principle | Application and evidence |
| --- | --- |
| Single responsibility | [Vector](../../src/math/vector.cpp) handles spatial arithmetic. [Image](../../src/image/image.cpp) stores pixels; [PPM](../../src/image/ppm.cpp) encodes and writes them. Camera creates rays, shapes intersect, Scene selects hits, and Renderer shades. |
| Open/closed | [Shape](../../src/geometry/shape.h) is the common intersection contract. Scene and Renderer do not branch on sphere versus plane. A new shape adds a new implementation. |
| Liskov substitution | Sphere and Plane return the nearest t strictly inside the same interval, or no result. Their normals are unit vectors with documented orientation. Tests cover clipped, missed and backward intersections for both shapes. |
| Interface segregation | Shape exposes one operation and a virtual destructor. No mesh, material editor or image-file operation is forced onto simple geometry. |
| Dependency inversion | Scene owns the small Shape abstraction. Renderer asks Scene for hits, and does not depend on Sphere or Plane. The executable composes concrete shapes. A separate scene-query interface is deferred until a second query implementation needs it. |
| DRY | Vector length reuses dot; normalisation reuses scaling. Test vector/colour comparisons share one tolerance. CI builds the development Dockerfile rather than maintaining another tool installation. Similar + and - formulas represent different knowledge and stay explicit. |
| Clean Code | The ray equation uses ordinary mathematical notation. Source and tests share domain folders. Helpers centralise CLI validation and output channel encoding. The obsolete greeting implementation and tests were removed after bootstrap. |

## Tradeoffs and future triggers

- Positions and directions share Vector. Introduce distinct types if invalid
  combinations become a recurring source of bugs.
- One core library is enough now. Split libraries when dependency boundaries or
  incremental build times become a concrete problem.
- Intersections scan all shapes. Measure before introducing a BVH or parallelism.
- A fixed world-space epsilon suits the demonstration. Revisit it for very small
  or very large scenes, and add tests at those scales first.
- RGB is linear until the output boundary. Gamma 2 and ambient light are teaching
  approximations; physically based materials and exact sRGB are future decisions.
- The simple quadratic formula is sufficient at the tested scale; near-tangent
  rays at extreme coordinates may need a more numerically stable solver.
- Library constructors validate dimensions, direction lengths and radii needed by
  the demo. General validation of every coordinate and colour channel is outside
  this CLI's fixed-scene scope.

The fixed camera, hard shadow edge and absence of antialiasing are explicit scope
choices. They are not evidence of physically accurate light transport.
