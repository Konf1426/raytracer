# Discovery report

**Project:** Raytracer  
**Author:** Melvin Becue  
**GitHub repository:** https://github.com/Konf1426/raytracer  
**Date:** November 16, 2026  
**Milestone:** m0 — Discovery and Research

---

## 1. Problem statement

A film studio in Los Angeles needs a reliable, maintainable, extensible and high-performance rendering system because its existing software is difficult to understand, debug and maintain, putting the production of an entire film at risk.

---

## 2. Client interview summary

### Introduction

The objective of the client interview is to understand the studio's real needs before making architectural or technical decisions.

The initial project brief explains that the studio needs a ray tracer written in C++, with readable code, automated tests, documentation and the possibility for partner studios to contribute to its development.

However, this initial description is not enough to define measurable requirements. The interview should clarify the current workflow, the problems experienced by the studio, its performance expectations and the constraints that will influence development.

The following sections distinguish information supplied in the project brief from questions requiring confirmation during the client interview.

**Interview status:** The actual interview transcript or notes have not yet been added to this report. The questions below are preparation and documentation prompts, not confirmed client quotations.

### Theme 1 — Current workflow and existing problems

**Need:** Understand how the studio currently produces rendered images and identify the main difficulties experienced with its rendering software.

The project brief indicates that previously generated rendering tools became difficult to maintain and unreliable. It does not explain which incidents occurred or how frequently they affected production.

**Interview question to document:**

> **Q (interviewer to be identified):** "Can you describe your current rendering workflow and the main problems you have experienced with your rendering tools?"
>
> **A:** [Insert the client's actual answer from the interview.]

**My conclusion:** Before proposing a new architecture, I need to understand which parts of the existing workflow cause the most problems. Reliability and maintainability are important objectives, but the specific failures and their consequences remain to be confirmed.

### Theme 2 — Performance and rendering time

**Need:** Identify measurable performance requirements for rendering individual frames and complete film sequences.

The client expects a renderer fast enough for film production, but the expression "fast enough" is not a measurable technical requirement.

**Interview question to document:**

> **Q (interviewer to be identified):** "What is the maximum acceptable rendering time for one frame, and what resolution do you normally use?"
>
> **A:** [Insert the client's actual answer from the interview.]

**My conclusion:** Rendering performance needs to be evaluated using measurable criteria, including frame resolution, rendering time, image quality and available computing resources.

For example, a 90-minute film at 24 frames per second contains 129,600 frames. If one machine requires one minute to calculate each frame, the total computation time would be 90 days without parallel processing.

This calculation is an illustrative example, not a confirmed requirement from the client.

### Theme 3 — Reliability and maintainability

**Need:** Understand what the studio considers reliable software and which engineering practices are necessary to maintain the renderer over time.

**Interview question to document:**

> **Q (interviewer to be identified):** "What types of software failures have caused production problems, and how do your developers currently investigate and fix them?"
>
> **A:** [Insert the client's actual answer from the interview.]

**My conclusion:** A maintainable renderer should have understandable components, automated tests, useful error messages, documentation and a reproducible build process.

These are proposed engineering practices based on the project brief, not additional requirements confirmed during the interview.

### Theme 4 — Collaboration and open-source contributions

**Need:** Understand how the studio expects external contributors and partner studios to participate in the project.

The project brief explicitly mentions that the software should be open to contributions from partner studios.

**Interview question to document:**

> **Q (interviewer to be identified):** "How do you expect partner studios to contribute to the renderer, and who will be responsible for reviewing and accepting changes?"
>
> **A:** [Insert the client's actual answer from the interview.]

**My conclusion:** The project will probably benefit from a clear contribution process, code review, documented interfaces and automated checks. These mechanisms could make external contributions easier to integrate without compromising software quality.

The contribution model and governance rules still need confirmation.

### Theme 5 — Features and project priorities

**Need:** Identify which capabilities are essential for the first usable version of the renderer and which can be developed later.

**Interview question to document:**

> **Q (interviewer to be identified):** "Which rendering features are essential for your first production use, and which features could be added in later versions?"
>
> **A:** [Insert the client's actual answer from the interview.]

**My conclusion:** Development should probably start with a small, testable rendering engine before introducing advanced features such as complex materials, indirect lighting, acceleration structures or distributed rendering.

The actual priorities must be based on the client's answers rather than assumptions.

### Open questions

The following questions remain open until they are answered or supported by interview notes:

1. What rendering software does the studio currently use?
2. What specific incidents motivated the development of a new renderer?
3. What image resolutions and output formats are required?
4. What is the maximum acceptable time to render one frame?
5. How many frames must the system process within a given deadline?
6. What hardware is available for rendering: CPU, GPU or render farm?
7. Which lighting, geometry and material features are mandatory?
8. What reliability and testing standards are expected?
9. How will partner studios contribute to the project?
10. Who will make architectural decisions and approve contributions?
11. What criteria will determine whether the first version is successful?
12. Are there constraints involving existing tools, file formats or pipelines?

**Important distinction:** The initial project brief provides the general objectives, but these unanswered questions must not be presented as confirmed client requirements.

---

## 3. State of the art

### Introduction

Before developing a new rendering engine, it is important to understand the solutions already available in the computer graphics industry.

Several professional rendering engines are capable of producing realistic images using ray tracing or path tracing. Some are commercial products designed for film and visual effects production, while others are open-source projects that allow developers to study and modify their implementations.

This comparison focuses on licensing, pricing, rendering capabilities and the potential relevance of each solution to the client's needs.

**Research date:** November 16, 2026.

### Comparison of existing solutions

| Solution | Maker | Licence | Pricing (checked November 16, 2026) | Capabilities | Sources |
|---|---|---|---|---|---|
| Autodesk Arnold | Autodesk | Proprietary, commercial subscription | Subscription required; price depends on the offer and region. No numerical price confirmed in the consulted official page. | Monte Carlo ray tracing, global illumination, realistic materials and rendering for film, television and visual effects. | [Official Arnold website](https://www.autodesk.com/products/arnold/overview), [Subscription](https://www.autodesk.com/products/arnold/buy) |
| Pixar RenderMan | Pixar Animation Studios | Proprietary, commercial; free non-commercial edition | $595 USD per commercial licence, with $250 USD annual maintenance. Free non-commercial edition available under its licence conditions. | Production rendering, physically based lighting, advanced materials, CPU/GPU rendering through XPU and integration into professional workflows. | [Official store](https://prod.renderman.pixar.com/store), [Licensing](https://prod.renderman.pixar.com/general-faq) |
| Blender Cycles | Blender Foundation / Blender community | Apache License 2.0 for the Cycles engine; Blender itself uses GPL | Free to use, including commercial use; no licence purchase required. | Physically based path tracing, CPU/GPU rendering, global illumination, reflections, refractions and material systems. | [Blender licensing](https://www.blender.org/about/license/), [Cycles source code](https://github.com/blender/cycles) |
| PBRT-v4 | PBRT project contributors | Apache License 2.0 | Free and open source; no licence purchase required. | Physically based rendering, spectral rendering, volumetric effects, CPU/GPU support and advanced light transport algorithms. | [Official project](https://www.pbrt.org/), [GitHub repository](https://github.com/mmp/pbrt-v4) |

Pricing observations:

- Prices and licensing terms are based on the official pages consulted on November 16, 2026.
- RenderMan's listed commercial price is in US dollars and may not represent the total cost of a large studio deployment.
- Arnold's numerical subscription price was not established from the official source consulted; it should not be estimated without verification.
- Open-source software may be free to license while still generating costs for hardware, engineering, support and maintenance.

### Autodesk Arnold

Arnold is a professional rendering engine used in computer graphics workflows involving complex scenes, realistic lighting and visual effects.

It uses Monte Carlo methods to simulate light transport and produce physically plausible images.

**Advantages:**

- Designed for professional production environments.
- Handles complex lighting and materials.
- Integrates with established 3D production tools.
- Provides a mature solution for photorealistic rendering.

**Limitations for this project:**

- It is proprietary software.
- Its internal source code is not generally available for unrestricted modification.
- The commercial licensing model creates dependencies on the vendor.

Arnold provides an example of the capabilities expected from a professional renderer, but its proprietary nature does not fully match the client's goal of creating an open, independently maintainable project.

### Pixar RenderMan

RenderMan is a rendering system developed by Pixar and used in professional animation and visual effects production.

Its capabilities include physically based rendering, advanced shading and production-oriented rendering workflows.

**Advantages:**

- Designed for large-scale film production.
- Provides sophisticated rendering technologies.
- Supports professional production workflows.
- Offers CPU/GPU rendering capabilities through its XPU architecture.

**Limitations for this project:**

- Its commercial version requires paid licences.
- It is proprietary rather than an unrestricted open-source project.
- The studio would remain dependent on the software provider for changes to the rendering engine itself.

RenderMan is an important reference for understanding the level of quality and functionality expected in film production.

### Blender Cycles

Cycles is an open-source path tracing engine associated with Blender.

Unlike proprietary renderers, its source code is publicly available, allowing developers to study its architecture and contribute improvements.

It supports realistic materials, reflections, refractions, indirect illumination and rendering on supported CPU and GPU hardware.

**Advantages:**

- Free and open source.
- Supports commercially usable rendering workflows.
- Provides realistic rendering capabilities.
- Allows developers to inspect and modify the rendering engine.
- Has an established development community.

**Limitations for this project:**

- Its architecture is relatively complex for a beginner.
- Integrating or adapting it may require significant technical knowledge.
- A large existing codebase can be difficult to understand without previous experience.

Cycles is particularly relevant because it demonstrates that open-source rendering technology can provide advanced features.

### PBRT-v4

PBRT stands for Physically Based Rendering. It is both a rendering implementation and a learning resource associated with the book *Physically Based Rendering: From Theory to Implementation*.

PBRT-v4 is written mainly in C++ and provides examples of sophisticated rendering algorithms.

**Advantages:**

- Open-source implementation.
- Designed around physically based rendering techniques.
- Includes support for spectral rendering and volumetric light transport.
- Provides both implementation code and extensive theoretical documentation.
- Useful for studying rendering architecture and algorithms.

**Limitations for this project:**

- The underlying mathematical concepts can be difficult to understand.
- Its advanced features exceed the requirements of an initial basic ray tracer.
- It is particularly valuable as a technical reference rather than a simple starting point for a new developer.

PBRT is one of the most useful resources for understanding the technical foundations of the project.

### Conclusions from the state of the art

The comparison shows that modern rendering engines share several important capabilities, including realistic lighting calculations, material simulation, scene management and performance optimisation.

Professional commercial solutions such as Arnold and RenderMan demonstrate the requirements of high-quality film rendering. However, they do not provide the same freedom to inspect, change and distribute their internal implementations as open-source software.

Open-source projects such as Cycles and PBRT demonstrate that complex rendering systems can be developed with publicly available source code.

The main challenge for the client is therefore not simply the ability to generate a realistic image. Existing software already provides this capability.

The distinctive requirement described in the brief is to develop a renderer that the studio can understand, test, maintain and extend over time, while allowing contributions from partner studios.

An important unanswered question is why adopting or adapting an existing open-source renderer would not satisfy the studio's needs. This should be investigated before committing to a completely new implementation.

For the first development stage, I would prioritise a simple, understandable architecture over an attempt to reproduce every feature of professional rendering engines.

Performance improvements and advanced rendering capabilities should be introduced progressively, guided by tests, measurements and confirmed requirements.

---

## 4. What is ray tracing?

Ray tracing is a computer graphics technique used to generate a two-dimensional image from a three-dimensional scene by simulating how light interacts with objects. The input is a scene containing geometry, materials, lights and a camera, along with settings such as image resolution. For each pixel, a renderer typically generates a ray from the virtual camera into the scene and calculates whether it intersects an object. If an intersection occurs, the renderer uses information about the surface, its material and the surrounding light to determine the pixel's colour. Additional rays may be generated to simulate shadows, reflections, refractions and indirect illumination. If no object is intersected, the renderer can use a background or environment colour. The calculated colours are eventually stored in an image. More advanced methods, such as path tracing, evaluate multiple possible light paths to produce realistic results. Increasing the number of rays or samples generally improves image quality, but also increases computation time. The main challenge is therefore to balance realism, rendering performance and implementation complexity.

### Main concepts

During my initial research, I identified several important concepts:

- **Ray:** A mathematical representation of a line or half-line defined by an origin and a direction.
- **Camera:** Defines the viewpoint from which the scene is rendered.
- **Scene:** Contains the objects, materials and lights required to generate an image.
- **Geometry:** Represents the shapes of objects, such as spheres, planes or triangles.
- **Intersection:** Determines whether a ray hits an object and where the collision occurs.
- **Surface normal:** A vector used to describe the orientation of a surface.
- **Material:** Defines how a surface interacts with incoming light.
- **Reflection:** Describes how light changes direction after hitting a reflective surface.
- **Refraction:** Describes how light changes direction when travelling between different materials.
- **Path tracing:** A rendering method that estimates light transport by sampling possible light paths.
- **Sampling:** The process of taking multiple measurements to estimate the final colour of a pixel.
- **Bounding Volume Hierarchy (BVH):** A data structure used to reduce the number of intersection tests required during rendering.

### Example of a simple rendering process

A basic ray tracer could follow these steps:

1. Create a virtual scene containing a camera, a sphere, a ground plane and a light source.
2. Configure the image resolution and camera position.
3. Generate a ray for each pixel.
4. Test whether the ray intersects the sphere or the ground.
5. Determine the closest visible intersection.
6. Calculate the surface colour based on material and lighting.
7. Optionally generate additional rays for shadows or reflections.
8. Store the resulting pixel colour.
9. Repeat until the image is complete.
10. Export the rendered image to a file.

At the beginning of the project, simple geometry and basic lighting should make it easier to verify the correctness of the implementation.

More advanced features can then be introduced progressively.

### Learning resources and references

**Getting started:**

[Ray Tracing in One Weekend — Peter Shirley, Trevor David Black and Steve Hollasch](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

This tutorial explains how to build a small ray tracer using C++. It is particularly relevant because it introduces the fundamental mathematical and programming concepts through a practical implementation.

It provides a way to understand ray generation, sphere intersections, materials and lighting before working on a larger architecture.

**Technical reference:**

[Physically Based Rendering: From Theory to Implementation, Fourth Edition](https://www.pbrt.org/)

This book explains the mathematical and algorithmic foundations of physically based rendering, alongside a C++ implementation.

It is more advanced than the introductory tutorial and will be useful when investigating more complex materials, light transport, acceleration structures and optimisation techniques.

---

## 5. What is C++?

### Programming paradigms

C++ is a compiled, statically typed programming language widely used in applications where performance, memory management and control over system resources are important.

It supports several programming paradigms.

**Procedural programming**

C++ allows developers to organise programs around functions, variables and sequences of instructions.

This approach can be useful for mathematical operations, such as calculating the intersection between a ray and a sphere.

**Object-oriented programming**

C++ supports classes, objects, encapsulation, inheritance and polymorphism.

These concepts can help represent the different components of a rendering engine.

For example, a `Sphere` class could represent a geometric object, while a `Camera` class could generate rays through the image.

**Generic programming**

C++ supports templates, which allow reusable functions and classes to work with different types.

For example, a generic mathematical component could support vectors with different numerical types.

**Functional programming concepts**

C++ also supports techniques such as lambda expressions, higher-order functions and immutable values.

These can be useful for small calculations and certain data-processing operations.

### Comparison with languages I already know

My previous development experience is mainly based on PHP, JavaScript and web frameworks such as Symfony.

C++ shares some concepts with these languages, but its execution and memory-management models are different.

| Concept | PHP / JavaScript | C++ |
|---|---|---|
| Typing | JavaScript is dynamically typed; PHP is dynamically typed with optional type declarations. | Statically typed, with compile-time type checking. |
| Execution | Usually executed through a runtime or interpreter/JIT environment. | Usually compiled into native machine code. |
| Memory management | Automatic memory management is generally provided by the runtime. | Object lifetimes, ownership and resource management require more explicit design. |
| Object-oriented programming | Supported by PHP and JavaScript. | Supported through classes, inheritance, encapsulation and polymorphism. |
| Performance | Depends on the runtime, implementation and workload. | Provides direct control over many low-level performance characteristics. |
| Pointers | Direct memory pointers are generally hidden from application developers. | Supports pointers, references and explicit memory-related operations. |
| Generic programming | Available through different language features and tooling. | Strongly supported through templates. |

One important difference is that C++ requires the developer to think carefully about the lifetime and ownership of objects.

In PHP or JavaScript, I can usually create an object without explicitly deciding when its memory will be released.

In C++, understanding object lifetime is an important part of writing correct software.

However, modern C++ also provides standard containers and smart pointers that make resource management safer and easier.

### What to watch out for

C++ provides a high level of control, but this also introduces risks that are less common in languages with automatic memory management.

**1. Memory management**

Incorrect memory handling can cause leaks, crashes or undefined behaviour.

A memory leak can occur when dynamically allocated memory is never released.

Modern C++ encourages the use of automatic object lifetimes, standard containers and smart pointers rather than unnecessary manual allocation.

**2. Pointers and references**

Pointers store memory addresses, while references provide another way to access existing objects.

Using invalid pointers or references can lead to serious bugs.

For example, accessing an object after its lifetime has ended can result in undefined behaviour.

**3. Undefined behaviour**

Some programming mistakes do not produce a predictable error.

Examples include accessing an array outside its bounds or dereferencing an invalid pointer.

Such errors can be difficult to investigate because the program may behave differently depending on the compiler, platform or optimisation settings.

**4. Compilation and linking**

Unlike the web development workflow I am familiar with, C++ commonly requires a compilation and linking process.

Dependencies, header files, compiler versions and build configurations must be managed carefully.

A build system such as CMake can help make compilation reproducible.

**5. Object ownership**

When multiple components refer to the same object, the code must clearly define who owns the object and how long it must remain alive.

Poor ownership design can lead to invalid references, unexpected changes or unnecessary memory usage.

**6. Performance optimisation**

C++ gives access to low-level optimisations, but this does not automatically make every program efficient.

I should first implement a correct version, create tests and collect measurements before attempting complex optimisations.

### Practices I would follow

For this project, I would use the following principles:

- Prefer standard library containers such as `std::vector`.
- Prefer automatic object lifetimes and RAII for resource management.
- Use `std::unique_ptr` when dynamic ownership is required and exclusive.
- Use `std::shared_ptr` only when shared ownership is justified.
- Avoid unnecessary raw owning pointers.
- Make interfaces clear and keep responsibilities separated.
- Use `const` where appropriate to prevent unintended modifications.
- Write tests for mathematical calculations and rendering behaviour.
- Enable compiler warnings and investigate them.
- Use debugging tools and sanitizers when possible.
- Measure performance before optimising.

These principles are particularly relevant for a renderer because the program may process large amounts of data and execute millions of mathematical operations.

### Shaping the domain model

The domain model describes how the concepts of the rendering problem are represented in code.

I would start with a small set of components representing the essential elements of a ray tracer.

| Component | Responsibility |
|---|---|
| `Vector3` | Represent three-dimensional vectors and mathematical operations. |
| `Ray` | Store the origin and direction of a ray. |
| `Camera` | Generate rays based on the camera position and image coordinates. |
| `Sphere` | Represent a sphere and calculate ray-sphere intersections. |
| `Plane` | Represent a flat surface and calculate intersections. |
| `Material` | Describe how a surface interacts with light. |
| `Light` | Represent a light source and its properties. |
| `Scene` | Group the geometry, materials and lights used for rendering. |
| `Renderer` | Coordinate ray generation, intersection calculations and pixel colour computation. |
| `Image` | Store pixel data and export the final rendering. |

For a more extensible architecture, an interface such as `Hittable` could define the common behaviour of objects that can intersect a ray.

A `Sphere` and a `Plane` could implement this interface differently while the renderer interacts with both through the same abstraction.

This would allow new geometric shapes to be introduced without requiring the rendering algorithm to know every concrete shape type.

However, I would avoid creating too many abstractions before they are necessary.

The first version should be simple enough to understand and test, while leaving room for future extensions.

### Applying software engineering principles

I would also apply the engineering principles introduced in the course.

**Single Responsibility Principle (SRP)**

Each component should have one main responsibility.

For example, a camera should generate rays, while an image writer should handle file output.

**Open/Closed Principle (OCP)**

The architecture should allow new geometry or materials to be introduced without unnecessarily rewriting existing components.

**Don't Repeat Yourself (DRY)**

Common mathematical operations should be implemented once and reused when possible.

**Encapsulation**

Internal data and implementation details should be protected behind clear interfaces.

**Testability**

Core operations such as vector calculations, ray-sphere intersections and pixel colour generation should be testable independently.

These practices should make the code easier to maintain and safer for future contributors.

### Learning resources and references

**Getting started:**

[LearnCpp.com](https://www.learncpp.com/)

This resource provides a structured introduction to modern C++, including variables, functions, classes, references, pointers, object lifetimes and memory management.

It is suitable for a developer who already knows other programming languages but needs to understand the specific rules and concepts of C++.

**Technical reference:**

[cppreference — C++ language reference](https://en.cppreference.com/w/cpp/language)

This documentation provides detailed information about C++ syntax, classes, templates, memory management, expressions and language features.

I would use it as a reference when implementing the renderer or investigating a specific behaviour.

**Additional practical resource:**

[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

This resource is particularly interesting because it combines C++ development with the exact rendering concepts required for this project.

---

## Conclusion

This discovery phase helped me understand that developing a ray tracer involves much more than writing an algorithm that generates an image.

The project combines several challenges: understanding a client's needs, studying existing rendering technologies, learning the fundamentals of computer graphics and developing maintainable software in C++.

The comparison of existing renderers shows that advanced rendering capabilities already exist in both commercial and open-source software.

Therefore, the project's value should not be measured only by visual quality or raw performance. Its maintainability, reliability, documentation, test coverage and ability to evolve are equally important according to the initial project brief.

The next step is to confirm the studio's actual requirements through the client interview, then develop a small, correct and testable rendering engine.

I would begin with basic geometric objects, ray intersections and simple lighting before introducing more advanced features.

This progressive approach should make it easier to understand the system, identify problems and improve performance without sacrificing code quality.

---

## References

All online sources below were consulted on November 16, 2026.

### Course documentation

1. Glassworks Tech — *Technical Strategy as a Tech-Lead*  
   https://learn.glassworks.tech/raytracer/

2. Glassworks Tech — *How to research this project*  
   https://learn.glassworks.tech/raytracer/veille-perspective/how-to-research/

3. Glassworks Tech — *Your discovery report*  
   https://learn.glassworks.tech/raytracer/veille-perspective/discovery-report/

### Rendering engines

4. Autodesk — *Arnold*  
   https://www.autodesk.com/products/arnold/buy

5. Pixar — *RenderMan Store and Licensing*  
   https://prod.renderman.pixar.com/store

6. Blender Foundation — *Blender License*  
   https://www.blender.org/about/license/

7. Blender — *Cycles Source Code*  
   https://github.com/blender/cycles

8. PBRT — *Physically Based Rendering*  
   https://www.pbrt.org/

9. PBRT — *PBRT-v4 Source Code*  
   https://github.com/mmp/pbrt-v4

### Ray tracing and C++

10. Peter Shirley, Trevor David Black and Steve Hollasch — *Ray Tracing in One Weekend*  
    https://raytracing.github.io/books/RayTracingInOneWeekend.html

11. Matt Pharr, Wenzel Jakob and Greg Humphreys — *Physically Based Rendering: From Theory to Implementation*  
    https://www.pbrt.org/

12. LearnCpp — *Modern C++ Tutorial*  
    https://www.learncpp.com/

13. cppreference — *C++ Language Reference*  
    https://en.cppreference.com/w/cpp/language
