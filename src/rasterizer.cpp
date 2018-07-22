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

void Rasterizer::drawLine(Vector3f &vertex1, Vector3f &vertex2,const Uint32 &color, Buffer<Uint32> *pixelBuffer ){
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

void Rasterizer::drawWireFrame(Vector3f *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer){
    drawLine(vertices[0], vertices[1], red, pixelBuffer);
    drawLine(vertices[1], vertices[2], green, pixelBuffer);
    drawLine(vertices[0], vertices[2], blue, pixelBuffer);
}  

//Draws triangles using baricentric coordinates,
void Rasterizer::drawTriangles(Vector3f *vertices, IShader &shader, Buffer<Uint32> *pixelBuffer, Buffer<float> *zBuffer){

    Rasterizer::viewportTransform(pixelBuffer, vertices);
    //Finding triangle bounding box limits & clips it to the screen width and height
    int xMax, xMin, yMax, yMin;
    Rasterizer::triBoundBox(xMax, xMin, yMax, yMin, vertices, pixelBuffer);

    //Per fragment variables            
    float depth, area;
    Vector3f bari, rgbVals, point;
    Vector3f zVals{vertices[0].z,vertices[1].z,vertices[2].z};
    area = 1/edge(vertices[0],vertices[1],vertices[2]);

    //Iterating through each pixel in triangle bounding box
    for(point.y = yMin; point.y <= yMax; ++point.y){
        for(point.x = xMin; point.x <= xMax; ++point.x){

            bari.x = edge(vertices[1], vertices[2], point);
            bari.y = edge(vertices[2], vertices[0], point);
            bari.z = edge(vertices[0], vertices[1], point);

            //Only draw if inside pixel
            if(bari.x >= 0 && bari.y >= 0 && bari.z >= 0 ){
                //Run fragment shader
                bari = bari* area;
                shader.fragment(bari, rgbVals, depth, zVals);

                //Zbuffer check
                if((*zBuffer)(point.x,point.y) < depth){
                    if(depth <= 1.0){
                        (*zBuffer)(point.x,point.y) = depth;
                        (*pixelBuffer)(point.x,point.y) = SDL_MapRGBA(mappingFormat,
                        rgbVals.data[0],rgbVals.data[1],rgbVals.data[2],0xFF);
                    }   
                }
            }
        }
    }
}  

void Rasterizer::viewportTransform(Buffer<Uint32> *pixelBuffer, Vector3f *vertices){
    for(int i = 0; i < 3; ++i){
        vertices[i].x = ((vertices[i].x + 1 ) * pixelBuffer->mWidth * 0.5);
        vertices[i].y = ((vertices[i].y + 1 ) * pixelBuffer->mHeight * 0.5);
    }
    //Swap because inverting y changes winding order

}

//Calculates baricentric coordinates of triangles using the cross product
void Rasterizer::barycentric(Vector3f &lambdas, float denom, float d00, float d01, float d11, float d20, float d21){
    lambdas.data[1] = denom  * ((d11*d20) - (d01*d21)); 
    lambdas.data[2] = denom  * ((d00*d21) - (d01*d20));
    lambdas.data[0] = 1.0f - lambdas.data[1] - lambdas.data[2];
}

void Rasterizer::triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin,Vector3f *vertices, Buffer<Uint32> *pixelBuffer){
    // xMax = std::ceil(std::max({vertices[0].x, vertices[1].x, vertices[2].x,}));
    // xMin = std::ceil(std::min({vertices[0].x, vertices[1].x, vertices[2].x,}));
    xMax = std::max({vertices[0].x, vertices[1].x, vertices[2].x});
    xMin = std::min({vertices[0].x, vertices[1].x, vertices[2].x});

    // yMax = std::ceil(std::max({vertices[0].y, vertices[1].y, vertices[2].y,}));
    // yMin = std::ceil(std::min({vertices[0].y, vertices[1].y, vertices[2].y,}));
    yMax = std::max({vertices[0].y, vertices[1].y, vertices[2].y});
    yMin = std::min({vertices[0].y, vertices[1].y, vertices[2].y});

    //Clip against screen
    xMax = std::min(xMax, pixelBuffer->mWidth -1);
    xMin = std::max(xMin, 0);

    yMax = std::min(yMax, pixelBuffer->mHeight -1);
    yMin = std::max(yMin, 0);
}


float Rasterizer::edge(Vector3f &a, Vector3f &b, Vector3f &c){
    return (b.x - a.x)*(c.y-a.y) - (b.y - a.y)*(c.x - a.x);
}


