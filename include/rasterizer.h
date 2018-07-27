#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "SDL.h"
#include "buffer.h"
#include "vector3D.h"
#include "shader.h"

//Shorthand of repeated types.
typedef std::array<float, 3> arr3f;
 
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

        //Proper triangle rasterization with vertex interpolation.
        static void drawTriangles(Vector3f *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer, Buffer<float> *zBuffer);

        //Transforms coordinates from NDC to pixel values
        static void viewportTransform(Buffer<Uint32> *pixelBuffer, Vector3f *vertices);

        static void triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin, Vector3f *vertices, Buffer<Uint32> *pixelBuffer);

        static float edge(Vector3f &a, Vector3f &b, Vector3f &c);

        static bool inside(float edge, float a, float b);

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
};



#endif