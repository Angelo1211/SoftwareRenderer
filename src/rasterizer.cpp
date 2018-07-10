#include "rasterizer.h"
#include "vector"
#include "array"
#include <algorithm>

const SDL_PixelFormat* Rasterizer::mappingFormat( SDL_AllocFormat(PIXEL_FORMAT));

void Rasterizer::makeCoolPattern(){
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);

    for(int y = 0; y < mPixelBuffer->mHeight; ++y){
        for(int x = 0; x < mPixelBuffer->mWidth; ++x){
            Uint8 r = x*y % 256;
            Uint8 g = y % 256;
            Uint8 b = x % 256;
            Uint32 color = SDL_MapRGBA(mappingFormat, r,g,b,0xFF);
            setPixelColor(x, y, color);
        }
    }
}

void Rasterizer::testPattern(){
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0xFF);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0xFF);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);
    setPixelColor(600,200, red);
    setPixelColor(400,200, green);
    setPixelColor(200,200, blue);

}

//Draws triangles using edge detection and baricentric coordinates,
//Also shades based on a light coming directly from the camera
// void Rasterizer::drawTriangles(Vector3 &a, Vector3 &b, Vector3 &c, float intensity ){
//     Uint32 color = SDL_MapRGBA(mappingFormat,
//                 255*intensity,255*intensity,255*intensity,0xFF);

//     //Converting to screen space
//     std::array<int, 3> xVerts;
//     std::array<int, 3> yVerts;
//     std::array<float, 3> zVerts;
//     xVerts[0] = (a.x + 1 ) * mCanvas->mWidth * 0.5;
//     yVerts[0] = (-a.y + 1 ) * mCanvas->mHeight * 0.5;
//     zVerts[0] = (a.z);

//     xVerts[1] = (b.x +1 ) * mCanvas->mWidth  * 0.5;
//     yVerts[1] = (-b.y +1 ) * mCanvas->mHeight * 0.5;
//     zVerts[1] = (b.z);

//     xVerts[2] = (c.x +1 ) * mCanvas->mWidth  * 0.5;
//     yVerts[2] = (-c.y +1 ) * mCanvas->mHeight * 0.5;
//     zVerts[2] = (c.z);


//     //Finding triangle bounding box
//     int xMax = *std::max_element(xVerts.begin(),xVerts.end());
//     int yMax = *std::max_element(yVerts.begin(),yVerts.end());
//     int xMin = *std::min_element(xVerts.begin(),xVerts.end());
//     int yMin = *std::min_element(yVerts.begin(),yVerts.end());

//     //Find triangle area
//     float area = std::abs((xVerts[2]-xVerts[0])*(yVerts[1]-yVerts[0]) 
//                             - (xVerts[1]-xVerts[0])*(yVerts[2]-yVerts[0]));

//     //Iterating the triangle bounding box
//     //printf("Iterating through triangle bounding box\n");
//     //printf("ymin: %d, ymax: %d, xmin: %d, xmax: %d\n",yMin, yMax, xMin, xMax);
//     for(int y = yMin; y <= yMax; ++y){
//         for(int x = xMin; x <= xMax; ++x){
//             //Throw pixel away if not in screen
//             if( x < 0 || x > mCanvas->mWidth || y < 0 || y > mCanvas->mHeight ) continue;

//             float lambda0 = ((x-xVerts[0])*(yVerts[1]-yVerts[0]) 
//                             - (xVerts[1]-xVerts[0])*(y-yVerts[0])) / area;

//             float lambda1 = ((x-xVerts[1])*(yVerts[2]-yVerts[1]) 
//                             - (xVerts[2]-xVerts[1])*(y-yVerts[1])) / area;

//             float lambda2 = ((x-xVerts[2])*(yVerts[0]-yVerts[2]) 
//                             - (xVerts[0]-xVerts[2])*(y-yVerts[2])) / area;

//             //Throw pixel away if not in triangle
//             if(lambda0 < 0 || lambda1 < 0 || lambda2 < 0) continue;

//             float depth = lambda1*zVerts[0] + lambda2*zVerts[1] + lambda0*zVerts[2];

//             //If any of the edge functions are smaller than zero, discard the point
            
//             if(getDepthBufferAtLocation(x,y) < depth){
//                 float depthDiff = std::abs(depth - getDepthBufferAtLocation(x,y));
//                 if(depth <= 1.0){
//                     if(depthDiff < 0.1){
//                     //printf("Bari: %f, %f, %f\n",lambda0,lambda1,lambda2);
//                     //printf("Depth Buffer: %f\n",getDepthBufferAtLocation(x,y));
//                     //printf("Pixel Depth: %f \n",depth);
//                     setDepthBufferAtLocation(x,y,depth);
//                     setPixelColor(x, y, color);

//                     }
//                     else{

//                         setDepthBufferAtLocation(x,y,depth);
//                         setPixelColor(x, y, color);
//                     }
//                 }
                
                
                    
//             } 

//         }
//     }

// }  

// void Rasterizer::drawWireFrame(Vector3 &v1, Vector3 &v2, Vector3 &v3 ){
//     drawLine(v1, v2, red);
//     drawLine(v2, v3, green);
//     drawLine(v1, v3, blue);
// }   

// void Rasterizer::drawLine(Vector3 &vertex1, Vector3 &vertex2, Uint32 &color){
//     int x1 = (vertex1.x + 1 ) * mCanvas->mWidth * 0.5;
//     int y1 = (-vertex1.y + 1 ) * mCanvas->mHeight * 0.5;
//     int x2 = (vertex2.x +1 ) * mCanvas->mWidth  * 0.5;
//     int y2 = (-vertex2.y +1 ) * mCanvas->mHeight * 0.5;

//     //transpose line if it is too steep
//     bool steep = false;
//     if (std::abs(x1-x2) < std::abs(y1-y2) ){
//         std::swap(x1,y1);
//         std::swap(x2,y2);
//         steep = true;
//     }

//     //Redefine line so that it is left to right
//     if ( x1  > x2 ){
//         std::swap(x1,x2);
//         std::swap(y1,y2);
//     }
//     int dx = x2 - x1;
//     int dy = y2 - y1;
//     int derror2 = std::abs(dy)*2;
//     int error2 = 0;
//     int y = y1;

//     for(int x=x1; x <= x2 ; x++){
//         if(steep){
//                 setPixelColor(y, x, color);
//             }
//             else{
//                 setPixelColor( x, y, color);
//             }
//         error2 += derror2;
//         if (error2 > dx){
//             y += (y2 > y1  ? 1 : -1);
//             error2 -= dx*2;
//         }

//     }
        
// }

void Rasterizer::setPixelColor(int x, int y, Uint32 color){
        int arrayCoordinates = convertCoordinates(x,y); 
        mPixelBuffer->buffer[arrayCoordinates] = color;
    
}

int Rasterizer::convertCoordinates(int x, int y){
    return ((y * mPixelBuffer->mWidth) + x);
}

float Rasterizer::getDepthBufferAtLocation(int x, int y){
    int arrayCoordinates = convertCoordinates(x,y);
    return mPixelBuffer->buffer[arrayCoordinates];
}

void Rasterizer::setDepthBufferAtLocation(int x, int y, float depth){
    int arrayCoordinates = convertCoordinates(x,y);
    mPixelBuffer->buffer[arrayCoordinates] = depth;
}

