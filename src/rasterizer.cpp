#include "rasterizer.h"

void Rasterizer::makeCoolPattern(){
    SDL_PixelFormat *mappingFormat = SDL_AllocFormat (PIXEL_FORMAT);
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);

    for(int y = 0; y < mCanvas->mHeight; ++y){
        for(int x = 0; x < mCanvas->mWidth; ++x){
            Uint8 r = x*y % 256;
            Uint8 g = y % 256;
            Uint8 b = x % 256;
            Uint32 color = SDL_MapRGBA(mappingFormat, r,g,b,0xFF);
            setPixelColor(color, x, y);
        }
    }
}

void Rasterizer::setPixelColor(Uint32 color, int x, int y){
    int arrayCoordinates = convertCoordinates(x,y);
    mCanvas->mBuffer[arrayCoordinates] = color;
}

int Rasterizer::convertCoordinates(int x, int y){
    return ((y * mCanvas->mWidth) + x);
}