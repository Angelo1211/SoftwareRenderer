# Simple Software Graphics Engine(SSGE): A real-time and interactive 3D Renderer built from scratch in C++

SSGE is the result of a two month long learning project with the aim of building a real-time 3D renderer from scratch in C++. It is capable of modern Physically Based Rendering without making use of any third party graphics libraries such as OpenGL, DirectX or Vulkan. It is entirely software based and does not utilize any hardware acceleration throughout it's rendering pipeline. SSGE can run on both Windows and Linux machines by leveraging CMAKE for executable building and compiling and SDL2 for OS and hardware-level abstraction.

Performance wise, it is capable of running scenes containing medium size meshes of ~50k triangles and 4 light sources at a relatively stable frame rate of 30fps (Of course, your mileage may vary) while running my most complex PBR shader. To achieve this it makes heavy use of multithreading, SIMD directives and various other optimizations such as early backface culling, frustrum culling and texture tiling among others.

You can check out a demo of it on your machine here:

* Windows demo
* Linux demo

## Samples
![Showcase](https://github.com/Angelo1211/SoftwareRenderer/blob/gh-pages/Images/SoftwareRenderer_057.png)   

## Motivation & Goals

I am planning on writing a blog post about this in more detail on my blog[LINK] soon but for now here's a quick summary of my goals coming into this project: 

#### Goals
1. Demonstrate my C++ Knowledge
2. Satisfy a life long curiosity about computer graphics
3. Keep the renderer Real-Time (<40ms)
4. Minimize external library usage to minimum
5. Multiplatform development
6. Use modern computer graphics techniques
7. Learn to manage and architect "large" projects

## Features

### Rendering 

* Parallelized forward renderer
* Programmable vertex & fragment shaders using C++ virtual functions
* Physically Based Shading 
* Metallic workflow 
* Cook-Torrance specular BRDF ( w/lambert diffuse)
* Perspective Correct Interpolation
* Tangent Space Normal Mapping
* Ambient Occlusion mapping
* Reverse (AKA logarithmic) Z-Buffering [1,0]
* Pre-vertex shader back-face Culling
* View Frustrum culling
* Gapless triangle rasterizer 
* Fast Gamma correction
* Directional Lighting
* Bilinear Texture Filtering
* Seamless texture repeat
* Flat, Gouraud, Phong, Blinn-Phong shaders (deprecated in favor of PBR)

### Engine

* SDL2 Backend
* Multiplatform executables
* Scene Switching
* Free moving camera 
* Orbiting Camera mode 
* Camera FOV controls
* Templated Vector Math / Linear algebra library 
* .Obj file parser
* Scene content outlined in .txt file
* Texture tiling to reduce cache misses
* Multithreading per object and vectorization within lighting shader
* Fully commented for future referencing
* Image loading through stb-image [LINK]
* Axis aligned Bounding Box generation and reconstruction

## Known Issues

* Moire Pattern and specular aliasing due to lack of texture minification
* Dark metallic objects due to lack of Image Based Lighting (or similar solution)
* Jaggies from lack of antialiasing 
* No shadows causing undesired illumination in some models
* Stuttering during slow rotations from lack of sub-pixel precision 


## References


