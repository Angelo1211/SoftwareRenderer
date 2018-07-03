#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "SDL.h"
#include "canvas.h"

class Rasterizer{

    public:
        static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA8888;
        Rasterizer(Canvas *canvas) :mCanvas(canvas){}

        Uint32 getPixelColor(int x, int y);

        void setPixelColor(Uint32 color, int x, int y);

        void makeCoolPattern();

    private:
        int convertCoordinates(int x, int y);
        Canvas * mCanvas;

};

#endif