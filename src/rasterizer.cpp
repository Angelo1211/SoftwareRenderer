#include "rasterizer.h"
#include "vector"
#include "array"
#include <algorithm>

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

//Should probably do something fancier in the future
void Rasterizer::drawTriangles(Vector3 &v0, Vector3 &v1, Vector3 &v2 ){
    std::array<int, 3> xVerts;
    std::array<int, 3> yVerts;
    xVerts[0] = (v0.x + 1 ) * mCanvas->mWidth * 0.5;
    yVerts[0] = (-v0.y + 1 ) * mCanvas->mHeight * 0.5;

    xVerts[1] = (v1.x +1 ) * mCanvas->mWidth  * 0.5;
    yVerts[1] = (-v1.y +1 ) * mCanvas->mHeight * 0.5;

    xVerts[2] = (v2.x +1 ) * mCanvas->mWidth  * 0.5;
    yVerts[2] = (-v2.y +1 ) * mCanvas->mHeight * 0.5;

    int xMax = *std::max_element(xVerts.begin(),xVerts.end());
    int yMax = *std::max_element(yVerts.begin(),yVerts.end());

    int xMin = *std::min_element(xVerts.begin(),xVerts.end());
    int yMin = *std::min_element(yVerts.begin(),yVerts.end());


    for(int y = yMin; y < yMax; ++y){
        for(int x = xMin; x < xMax; ++x){
            float edge1 = (x-xVerts[0])*(yVerts[1]-yVerts[0]) 
                            - (xVerts[1]-xVerts[0])*(y-yVerts[0]);
            float edge2 = (x-xVerts[1])*(yVerts[2]-yVerts[1]) 
                            - (xVerts[2]-xVerts[1])*(y-yVerts[1]);
            float edge3 = (x-xVerts[2])*(yVerts[0]-yVerts[2]) 
                            - (xVerts[0]-xVerts[2])*(y-yVerts[2]);

            if(edge1 >= 0 && edge2 >= 0 && edge3 >= 0){
                setPixelColor(white, x, y);
            } 
        
        }
    }
    
}   

void Rasterizer::drawWireFrame(Vector3 &v1, Vector3 &v2, Vector3 &v3 ){
    drawLine(v1, v2, red);
    drawLine(v2, v3, green);
    drawLine(v1, v3, blue);
}   

void Rasterizer::drawLine(Vector3 &vertex1, Vector3 &vertex2, Uint32 &color){

        int x1 = (vertex1.x + 1 ) * mCanvas->mWidth * 0.5;
        int y1 = (-vertex1.y + 1 ) * mCanvas->mHeight * 0.5;
        int x2 = (vertex2.x +1 ) * mCanvas->mWidth  * 0.5;
        int y2 = (-vertex2.y +1 ) * mCanvas->mHeight * 0.5;

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
