#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "SDL.h"
#include "canvas.h"
#include "model.h"

class Rasterizer{

    public:
        Rasterizer(Canvas *canvas) :mCanvas(canvas){}

        void drawModels(Model * models);

        void testPattern();

        void makeCoolPattern();

    private:
        static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA8888;
        
        static const SDL_PixelFormat* mappingFormat;

        Uint32 getPixelColor(int x, int y);

        void drawLine(Vector3 vertex1, Vector3 vertex2, Uint32 color);

        void setPixelColor(Uint32 color, int x, int y);

        int convertCoordinates(int x, int y);

        Canvas * mCanvas;

};

#endif