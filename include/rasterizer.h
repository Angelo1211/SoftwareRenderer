#ifndef RASTERIZER_H
#define RASTERIZER_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-03
// PURPOSE      : To break down the vertex data into fragments. Determine which of those
//                fragments are visible within the screen and within a given line/triangle.
//                apply the fragment shader to each triangle and post process using gamma
//                correction. Can rasterize points, lines and triangles.
// ===============================
// SPECIAL NOTES: Although line rasterization is possible the engine currently only supports
// triangle rasterization through the higher level software rendering class. It's important
// to also note that this rasterizer is nowhere close to optimized. On a future iteration,
// it would an ideal candidate for a total makeover, reducing the coupling with other classes
// and transforming the rasterizer pipeline from a float and scalar one to a fixed point 
// and vectorized one. This would simplify the implementation of of many advanced features
// such as mip-mapping, sub-pixel precision, gradient based shading and antialiasing which
// are now very complicated to implement without it.
// ===============================

//Headers
#include "SDL.h"
#include "buffer.h"
#include "vector3D.h"
#include "shader.h"
 
//Takes in vertex data, rasterizes the surface and applies the fragment shader at
//each fragment. If it passes the depth test the fragment is written to the pixel buffer.
class Rasterizer{
    public:
        //Simple full screen effects that don't need any vertex data
        static void makeCoolPattern(Buffer<Uint32> *pixelBuffer);
        static void testPattern(Buffer<Uint32> *pixelBuffer);

        //Bresenham's line drawing algorithm using only int arithmetic
        static void drawLine(Vector3f &vertex1, Vector3f &vertex2, const Uint32 &color, Buffer<Uint32> *pixelBuffer);

        //Draws wireframe rendering of triangle by calling the line drawer for each
        //Line in a triangle.(AB, BC, AC)
        static void drawWireFrame(Vector3f *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer);

        //Proper triangle rasterization with vertex interpolation. Zbuffering, post processing
        //Sadly, not very efficient since it uses floats and scalar operations instead of SIMD
        static void drawTriangles(Vector3f *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer, Buffer<float> *zBuffer);

        //Transforms coordinates from NDC to pixel values
        static void viewportTransform(Buffer<Uint32> *pixelBuffer, Vector3f *vertices);

        //Builds the triangle bounding box
        static void triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin, Vector3f *vertices, Buffer<Uint32> *pixelBuffer);

        //Baricentric coordinates by use of pineda style edge detection
        static float edge(Vector3f &a, Vector3f &b, Vector3f &c);
        static bool inside(float edge, float a, float b);

        //Post processing operations
        static float clamp(float n, float lower, float upper);
        static int gammaAdjust(float n);

    private:
        Rasterizer(){}; //Ensuring an object can never be instanced accidentally

        //Setting this equal to the same pixel format our screen texture is in
        static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGB888;
        static const SDL_PixelFormat* mappingFormat;

        //Some basic colors
        static const Uint32 white;
        static const Uint32 red;
        static const Uint32 green;
        static const Uint32 blue; 

        //Gamma correction look-up table for gamma = 2.2
        static const int gammaTable[256];
};

#endif