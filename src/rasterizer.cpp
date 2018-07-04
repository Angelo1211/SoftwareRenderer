#include "rasterizer.h"
#include "vector"

const SDL_PixelFormat* Rasterizer::mappingFormat( SDL_AllocFormat(PIXEL_FORMAT));

void Rasterizer::makeCoolPattern(){
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

void Rasterizer::testPattern(){
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);
    setPixelColor(red,600,200);
    setPixelColor(green,400,200);
    setPixelColor(blue,200,200);

}

void Rasterizer::drawModels(Model * models){
    Uint32 white = SDL_MapRGBA(mappingFormat, 0xFF,0xFF,0xFF,0xFF);
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);
    Mesh * modelMesh = models->getMesh();
    std::vector<Vector3> * faces = &modelMesh->faces;
    std::vector<Vector3> * vertices = &modelMesh->vertices;
    for (Vector3 f : *faces ){
        Vector3 v1 = (*vertices)[f.x-1];
        Vector3 v2 = (*vertices)[f.y-1];
        Vector3 v3 = (*vertices)[f.z-1];
        drawLine(v1, v2, red);
        drawLine(v2, v3, green);
        drawLine(v1, v3, blue);
    }

}   

void Rasterizer::drawLine(Vector3 vertex1, Vector3 vertex2, Uint32 color){
        int scalingFactor = 90;
        int x1 = (vertex1.x  * scalingFactor) + mCanvas->mWidth * 1 / 2;
        int y1 = (-vertex1.y  * scalingFactor) + mCanvas->mHeight * 2 / 3;
        int x2 = (vertex2.x  * scalingFactor) + mCanvas->mWidth  * 1 / 2;
        int y2 = (-vertex2.y  * scalingFactor) + mCanvas->mHeight * 2 / 3;
        
        //transpose line if it is too steep
        bool steep = false;
        if (std::abs(x1-x2) < std::abs(y1-y2) ){
            std::swap(x1,y1);
            std::swap(x2,y2);
            steep = true;
        }

        //Redefine line so that it is left to right
        if ( x1  > x2 ){
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        int dx = x2 - x1;
        int dy = y2 - y1;
        int derror2 = std::abs(dy)*2;
        int error2 = 0;
        int y = y1;

        for(int x=x1; x <= x2 ; x++){
            if(steep){
                    setPixelColor(color, y, x);
                }
                else{
                    setPixelColor(color, x, y);
                }
            error2 += derror2;
            if (error2 > dx){
                y += (y2 > y1  ? 1 : -1);
                error2 -= dx*2;
            }

        }
        
}

void Rasterizer::setPixelColor(Uint32 color, int x, int y){
    int arrayCoordinates = convertCoordinates(x,y);
    if( ((x >= 0 ) && (y >= 0)) && ((x < mCanvas->mWidth ) && (y < mCanvas->mHeight))  ){   
    mCanvas->mBuffer[arrayCoordinates] = color;
    }
    
}

int Rasterizer::convertCoordinates(int x, int y){
    return ((y * mCanvas->mWidth) + x);
}
