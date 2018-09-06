# Simple Software Graphics Engine(SSGE): A real-time and interactive 3D Renderer built from scratch in C++

SSGE is the result of a two month long learning project with the aim of building a real-time 3D renderer from scratch in C++. It is capable of modern Physically Based Rendering(PBR) without making use of any third party graphics libraries such as OpenGL, DirectX or Vulkan. It is entirely software based and does not utilize any hardware acceleration throughout it's rendering pipeline. SSGE can run on both Windows and Linux machines by leveraging CMAKE for executable building and compiling and SDL2 for OS and hardware-level abstraction.

Performance wise, it is capable of running scenes containing medium size meshes of ~50k triangles and 4 light sources at a relatively stable frame rate of 30fps (Of course, your mileage may vary) while running the very demanding PBR shader. To achieve this it makes heavy use of multithreading, SIMD directives and various other optimizations such as early backface culling, frustum culling and texture tiling among others.

You can download a demo of it for your machine here:

* [Windows demo](https://drive.google.com/drive/folders/1IlvRr0weQiB42MVNkHjej-9scYlN7rdb)
* [Linux demo](https://drive.google.com/drive/folders/1IlvRr0weQiB42MVNkHjej-9scYlN7rdb)

## Render Samples

![Louis XIV de France, Louvre, Paris. Scan by HoangHiepVu](https://raw.githubusercontent.com/Angelo1211/SoftwareRenderer/gh-pages/Images/SoftwareRenderer_062.png)

![Utah teapots with 5 different materials](https://raw.githubusercontent.com/Angelo1211/SoftwareRenderer/gh-pages/Images/IntroTeapots.png)

![Chest PBR Test model from @Emackey](https://raw.githubusercontent.com/Angelo1211/SoftwareRenderer/gh-pages/Images/SoftwareRenderer_064.png)

![Cerberus PBR test model by Andrew Maximov](https://raw.githubusercontent.com/Angelo1211/SoftwareRenderer/gh-pages/Images/SoftwareRenderer_057.png)   

![Orbiting the chest](https://github.com/Angelo1211/SoftwareRenderer/blob/gh-pages/Images/chest2.gif?raw=true)


## Development & Feedback

I am planning on starting a series of blog posts sometime soon that will explain some of the cooler components of the engine and how I went about implementing them. I also feel it would be helpful to other beginners if I were to elaborate on some of the components that gave me the most trouble, such as how to design the general architecture of the engine, how to profile, optimize and bug fix your code and lastly how I personally tackled learning Computer Graphics coming from a non-cs background. 

#### Goals
1. Demonstrate my C++ Knowledge
2. Satisfy a life long curiosity about computer graphics
3. Keep the renderer Real-Time (original goal was 16ms actual ~29ms)
4. Minimize external library usage to minimum
5. Multiplatform development
6. Use modern computer graphics techniques
7. Learn to self-manage and architect "large" projects

I set these goals above as my learning targets for the project before I began and I have to say that I am very happy with how much progress I managed to make with most of them. If you're interested, I gave a talk that went more in detail about this and the journey in itself and you can find the slides for it here:

### Talk [Slides](https://docs.google.com/presentation/d/e/2PACX-1vTHXIVnes1OMQwN2EEg8sRDp4TVkgDd3IsLrV8Rdxzk-YlKkhn0JzzeOTrtS6lqpmT1shfQzxf5UGiF/pub?start=false&loop=false&delayms=3000)

## Main Features

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
* Image loading through [stb-image](https://github.com/nothings/stb)
* Axis aligned Bounding Box generation and reconstruction

## Known Issues

* Moire Pattern and specular aliasing due to lack of texture minification
* Dark metallic objects due to lack of Image Based Lighting (or similar solution)
* Jaggies from lack of antialiasing 
* No shadows causing undesired illumination in some models
* Stuttering during slow rotations from lack of sub-pixel precision 


## References

I've moved most of the sources into a separate page [on the wiki](https://github.com/Angelo1211/SoftwareRenderer/wiki/Rendering-References) for easier navigation and indexing. However, here are some of the ones I utilized the most:

* [EDAN35 - High Performance Computer Graphics](http://cs.lth.se/edan35/)
* [Stanford CS248, Spring 2018 - Interactive Computer Graphics](http://graphics.stanford.edu/courses/cs248-18-spring/)
* [ScratchAPixel](https://www.scratchapixel.com/)
* [Learn OpenGL](https://learnopengl.com/Introduction)
* [Tiny Renderer](https://github.com/ssloy/tinyrenderer)

## Model & Texture Acknowledgements

* [Cerberus Gun Mesh & Textures by Andrew Maximov](http://artisaverb.info/Cerberus.html)
* [Utah Teapot with closed lid Mesh by Nik Clark](https://en.wikipedia.org/wiki/Utah_teapot#/media/File:Utah_teapot_(solid).stl)
* [Fire Hydrant & Chest by @emackey PBR test models](https://emackey.github.io/testing-pbr/)
* [Louis XIV de France, Louvre, Paris by HoangHiepVu](https://sketchfab.com/models/a0cc0e7eee384c99838dff2857b8158c)
* [Gold, Marble, Painted Metal by CC0Textures.com](https://cc0textures.com/home)
* [Azulejos Texture by João Paulo](https://3dtextures.me/2018/07/09/azulejos-tiles-001/)
* [Elephant, Dragon, Bunny Mesh models from Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/)

If any of the above textures or meshes have been mis-acknowledged, are misattributed, or missing a proper reference please send me a DM on Twitter or email me at the address inside the source files and I'll fix it immediately.

## License

MIT License

Copyright (c) 2018 Angel Ortiz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.






