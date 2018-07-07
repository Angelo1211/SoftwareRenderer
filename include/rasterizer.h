#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "SDL.h"
#include "canvas.h"
#include "model.h"

class Rasterizer{

    public:
        Rasterizer(Canvas *canvas) :mCanvas(canvas){}

        void drawTriangles(Vector3 &v1, Vector3 &v2, Vector3 &v3, float intensity);

        void drawWireFrame(Vector3 &v1, Vector3 &v2, Vector3 &v3);

        void testPattern();

        void makeCoolPattern();

    private:
        static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA8888;
        
        static const SDL_PixelFormat* mappingFormat;

        Uint32 getPixelColor(int x, int y);

        void drawLine(Vector3 &vertex1, Vector3 &vertex2, Uint32 &color);

        void setPixelColor(Uint32 color, int x, int y);

        int convertCoordinates(int x, int y);

        Canvas * mCanvas;

        Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF,0xFF,0xFF,0xFF);
        Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
        Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
        Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);

};

#endif