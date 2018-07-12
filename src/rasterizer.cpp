#include "rasterizer.h"
#include "vector"
#include "array"
#include <algorithm>

const SDL_PixelFormat* Rasterizer::mappingFormat( SDL_AllocFormat(PIXEL_FORMAT));
const Uint32 Rasterizer::white(SDL_MapRGBA(mappingFormat, 0xFF,0xFF,0xFF,0xFF));
const Uint32 Rasterizer::red(SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF));
const Uint32 Rasterizer::green(SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF));
const Uint32 Rasterizer::blue(SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF));

void Rasterizer::makeCoolPattern(Buffer<Uint32> *pixelBuffer){
    for(int y = 0; y < pixelBuffer->mHeight; ++y){
        for(int x = 0; x < pixelBuffer->mWidth; ++x){
            Uint8 r = x*2 % 256;
            Uint8 g = y/8 % 256;
            Uint8 b = r*y % 256;
            Uint32 color = SDL_MapRGBA(mappingFormat, r,g,b,0xFF);
            (*pixelBuffer)(x,y) = color;
        }
    }
}

void Rasterizer::testPattern(Buffer<Uint32> *pixelBuffer){
    (*pixelBuffer)(600,200) = red;
    (*pixelBuffer)(400,200) = green;
    (*pixelBuffer)(200,200) = blue;

}

void Rasterizer::drawLine(Vector3 &vertex1, Vector3 &vertex2,const Uint32 &color, Buffer<Uint32> *pixelBuffer ){
    int x1 = (vertex1.x + 1 ) * pixelBuffer->mWidth * 0.5;
    int y1 = (-vertex1.y + 1 ) * pixelBuffer->mHeight * 0.5;
    int x2 = (vertex2.x +1 ) * pixelBuffer->mWidth  * 0.5;
    int y2 = (-vertex2.y +1 ) * pixelBuffer->mHeight * 0.5;

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
                (*pixelBuffer)(y, x) = color;
            }
            else{
                (*pixelBuffer)(x,y) = color;
            }
        error2 += derror2;
        if (error2 > dx){
            y += (y2 > y1  ? 1 : -1);
            error2 -= dx*2;
        }

    }
        
}

void Rasterizer::drawWireFrame(Vector3 *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer){
    drawLine(vertices[0], vertices[1], red, pixelBuffer);
    drawLine(vertices[1], vertices[2], green, pixelBuffer);
    drawLine(vertices[0], vertices[2], blue, pixelBuffer);
}  


//Draws triangles using edge detection and baricentric coordinates,
//Also shades based on a light coming directly from the camera
void Rasterizer::drawTriangles(Vector3 *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer, Buffer<float> *zBuffer, float intensity){
    Uint32 color = SDL_MapRGBA(mappingFormat,
                255*intensity,255*intensity,255*intensity,0xFF);

    //Converting to screen space
    std::array<int, 3> xVerts;
    std::array<int, 3> yVerts;
    std::array<float, 3> zVerts;
    xVerts[0] = (vertices[0].x + 1 ) * pixelBuffer->mWidth * 0.5;
    yVerts[0] = (-vertices[0].y + 1 ) * pixelBuffer->mHeight * 0.5;
    zVerts[0] = vertices[0].z;

    xVerts[1] = (vertices[1].x +1 ) * pixelBuffer->mWidth  * 0.5;
    yVerts[1] = (-vertices[1].y +1 ) * pixelBuffer->mHeight * 0.5;
    zVerts[1] = vertices[1].z;

    xVerts[2] = (vertices[2].x +1 ) * pixelBuffer->mWidth  * 0.5;
    yVerts[2] = (-vertices[2].y +1 ) * pixelBuffer->mHeight * 0.5;
    zVerts[2] = vertices[2].z;


    //Finding triangle bounding box
    int xMax = *std::max_element(xVerts.begin(),xVerts.end());
    int yMax = *std::max_element(yVerts.begin(),yVerts.end());
    int xMin = *std::min_element(xVerts.begin(),xVerts.end());
    int yMin = *std::min_element(yVerts.begin(),yVerts.end());

    //Find triangle area
    float area = std::abs((xVerts[2]-xVerts[0])*(yVerts[1]-yVerts[0]) 
                            - (xVerts[1]-xVerts[0])*(yVerts[2]-yVerts[0]));

    //Iterating the triangle bounding box
    //printf("Iterating through triangle bounding box\n");
    //printf("ymin: %d, ymax: %d, xmin: %d, xmax: %d\n",yMin, yMax, xMin, xMax);
    for(int y = yMin; y <= yMax; ++y){
        for(int x = xMin; x <= xMax; ++x){
            //Throw pixel away if not in screen
            if( x < 0 || x > pixelBuffer->mWidth || y < 0 || y > pixelBuffer->mHeight ) continue;

            float lambda0 = ((x-xVerts[0])*(yVerts[1]-yVerts[0]) 
                            - (xVerts[1]-xVerts[0])*(y-yVerts[0])) / area;

            float lambda1 = ((x-xVerts[1])*(yVerts[2]-yVerts[1]) 
                            - (xVerts[2]-xVerts[1])*(y-yVerts[1])) / area;

            float lambda2 = ((x-xVerts[2])*(yVerts[0]-yVerts[2]) 
                            - (xVerts[0]-xVerts[2])*(y-yVerts[2])) / area;

            //Throw pixel away if not in triangle
            if(lambda0 < 0 || lambda1 < 0 || lambda2 < 0) continue;

            float depth = lambda1*zVerts[0] + lambda2*zVerts[1] + lambda0*zVerts[2];

            //If any of the edge functions are smaller than zero, discard the point
            
            if((*zBuffer)(x,y) < depth){
                if(depth <= 1.0){
                        (*zBuffer)(x,y) = depth;
                        (*pixelBuffer)(x,y) = color;
                }   
            } 

        }
    }

}  

 





