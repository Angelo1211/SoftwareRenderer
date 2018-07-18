#include "rasterizer.h"
#include "vector"
#include "array"
#include <algorithm>

//Initializing all the basic colors 
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
    //NDC to viewport transform
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

    //Redefined to use only int arithmetic
    int dx = x2 - x1;
    int dy = y2 - y1;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y1;

    for(int x=x1; x <= x2 ; x++){
        if(steep){
            (*pixelBuffer)(y, x) = color; //Swap back because of steep transpose
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

//Draws triangles using baricentric coordinates,
void Rasterizer::drawTriangles(Vector3 *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer, Buffer<float> *zBuffer){

    //Converting to viewport space 
    std::array<int, 3>   xVerts;
    std::array<int, 3>   yVerts;
    Vector3 zVerts;
    Rasterizer::viewportTransform(pixelBuffer, vertices, xVerts, yVerts, zVerts);

    //Finding triangle bounding box limits clips it to the screen dimension
    //Whenever I get to clipping this will probably prove to be unnecessary 
    int xMax = *std::max_element(xVerts.begin(),xVerts.end());
    xMax = (xMax > pixelBuffer->mWidth) ? pixelBuffer->mWidth : xMax;

    int xMin = *std::min_element(xVerts.begin(),xVerts.end());
    xMin = (xMin < 0) ? 0 : xMin;

    int yMax = *std::max_element(yVerts.begin(),yVerts.end());
    yMax = (yMax > pixelBuffer->mHeight) ? pixelBuffer->mHeight : yMax;

    int yMin = *std::min_element(yVerts.begin(),yVerts.end());
    yMin = (yMin < 0) ? 0 : yMin;

    //Find triangle area
    float invArea = 1/(float)((xVerts[2]-xVerts[0])*(yVerts[1]-yVerts[0]) 
                            - (xVerts[1]-xVerts[0])*(yVerts[2]-yVerts[0]));     
    //Per fragment variables            
    float depth = 0;
    Vector3 lambdas;
    Vector3 rgbVals;

    //Iterating through triangle bounding box
    for(int y = yMin; y <= yMax; ++y){
        for(int x = xMin; x <= xMax; ++x){
            
            Rasterizer::barycentric(lambdas, invArea, x, y, xVerts, yVerts);

            //Throw pixel away if not in triangle
            if(lambdas.data[0] < 0 || lambdas.data[1] < 0 || lambdas.data[2] < 0) continue;

            //Run fragment shader
            shader.fragment(lambdas, rgbVals, depth, zVerts);

            //Zbuffer check
            if((*zBuffer)(x,y) < depth){
                if(depth <= 1.0){
                    (*zBuffer)(x,y) = depth;
                    (*pixelBuffer)(x,y) = SDL_MapRGBA(mappingFormat,
                rgbVals.data[0],rgbVals.data[1],rgbVals.data[2],0xFF);
                }   
            } 
        }
    }
}  

void Rasterizer::viewportTransform(Buffer<Uint32> *pixelBuffer, Vector3 *vertices,std::array<int, 3>   &xV,std::array<int, 3>   &yV, Vector3   &zV){
    for(int i = 0; i < 3; ++i){
        xV[i] = (int)((vertices[i].x + 1 ) * pixelBuffer->mWidth * 0.5);
        yV[i] = (int)((-vertices[i].y + 1 ) * pixelBuffer->mHeight * 0.5);
        zV.data[i] = vertices[i].z;
    }
}

//Calculates baricentric coordinates of triangles using the cross
void Rasterizer::barycentric(Vector3 &lambdas, float invArea, int x, int y,  
                        std::array<int, 3>   &xVerts, std::array<int, 3>   &yVerts){
    for(int i = 0; i < 3; ++i){
        int i2 = (i+1)%3;
        lambdas.data[i] = invArea * ((x-xVerts[i])*(yVerts[i2]-yVerts[i]) 
                            - (xVerts[i2]-xVerts[i])*(y-yVerts[i]));
    }
}




