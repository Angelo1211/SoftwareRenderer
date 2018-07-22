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

    //Converting to viewport space 
    arr3i  xV, yV;
    Vector3f zVerts;
    Rasterizer::viewportTransform(pixelBuffer, vertices, xV, yV, zVerts);

    //Finding triangle bounding box limits & clips it to the screen width and height
    int xMax, xMin, yMax, yMin;
    Rasterizer::triBoundBox(xMax, xMin, yMax, yMin, xV, yV, pixelBuffer);

    //Per triangle variables
    Vector3i v0(xV[1]-xV[0], yV[1]-yV[0], 0);
    Vector3i v1(xV[2]-xV[0], yV[2]-yV[0], 0);
    int d00 = v0.dot2D(v0);
    int d01 = v0.dot2D(v1);
    int d11 = v1.dot2D(v1);
    //printf("%d, %d, %d\n",d00, d01, d11);
    float denom = 1.0 / (float)((d00 * d11) - (d01 *d01));
    if (denom == INFINITY) return;
    //printf("%lf\n", denom);
    //Per fragment variables            
    float depth = 0;
    int d20   = 0;
    int d21   = 0;
    Vector3f lambdas;
    Vector3f rgbVals;
    Vector3i v2;

    //Iterating through each pixel in triangle bounding box
    for(int y = yMin; y <= yMax; ++y){
        for(int x = xMin; x <= xMax; ++x){
            v2.x = x-xV[0];
            v2.y = y-yV[0];
            d20 = v2.dot2D(v0);
            d21 = v2.dot2D(v1);
            Rasterizer::barycentric(lambdas, denom, d00, d01, d11, d20, d21);

            // if (lambdas.data[0] < 0 && lambdas.data[0] > -0.1 ){
            //     lambdas.print();
            // }

            //Throw pixel away if not in triangle
            //if( lambdas.y < 0 || lambdas.z < 0  || lambdas.x < 0 ) continue;
            if( lambdas.y > 1 || lambdas.y < 0 || lambdas.z > 1 || lambdas.z < 0  || ((lambdas.y + lambdas.z) > 1) ) continue;
            //if( (lambdas.x >= 0) && (lambdas.y >= 0) && (lambdas.z >= 0)){

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

void Rasterizer::viewportTransform(Buffer<Uint32> *pixelBuffer, Vector3f *vertices,arr3i   &xV,arr3i  &yV, Vector3f   &zV){
    for(int i = 0; i < 3; ++i){
        xV[i] = ((vertices[i].x + 1 ) * pixelBuffer->mWidth * 0.5);
        yV[i] = ((-vertices[i].y + 1 ) * pixelBuffer->mHeight * 0.5);
        zV.data[i] = vertices[i].z;

        printf("%f , %f \n", vertices[i].x, ((vertices[i].x + 1 ) * pixelBuffer->mWidth * 0.5));
    }
}

//Calculates baricentric coordinates of triangles using the cross product
void Rasterizer::barycentric(Vector3f &lambdas, float denom, float d00, float d01, float d11, float d20, float d21){
    lambdas.data[1] = denom  * ((d11*d20) - (d01*d21)); 
    lambdas.data[2] = denom  * ((d00*d21) - (d01*d20));
    lambdas.data[0] = 1.0f - lambdas.data[1] - lambdas.data[2];
}

void Rasterizer::triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin, arr3i &xV, arr3i &yV, Buffer<Uint32> *pixelBuffer){
    xMax = *std::max_element(xV.begin(),xV.end());
    xMax = (xMax > pixelBuffer->mWidth) ? pixelBuffer->mWidth -1 : xMax;

    xMin = *std::min_element(xV.begin(),xV.end());
    xMin = (xMin < 0) ? 0 : xMin;

    yMax = *std::max_element(yV.begin(),yV.end());
    yMax = (yMax > pixelBuffer->mHeight) ? pixelBuffer->mHeight -1 : yMax;

    yMin = *std::min_element(yV.begin(),yV.end());
    yMin = (yMin < 0) ? 0 : yMin;
}


