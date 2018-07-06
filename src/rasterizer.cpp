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

    //View Matrix stuff, to be moved elsewhere
    //Represents a camera rotating around the origin
    float t = static_cast<float>(SDL_GetTicks());
    float radius = 8;
    float camX   = std::sin(t/5000) * radius;
    float camZ   = std::cos(t/5000) * radius;
    Vector3 pos(camX, 0, camZ);
    Vector3 tar;
    Vector3 v(0,1,0);
    Matrix4 viewMatrix = Matrix4::lookAt(pos,tar,v);
    viewMatrix = Matrix4::makeTranslateMat(0,camX,0)*viewMatrix;

    //Creating perspective matrix
    //No need to build it each frame since fov and near far clipping is normally
    //Not something that changes every frame
    float fov = 90;
    float aspectRatio = mCanvas->mWidth / (float)mCanvas->mHeight;
    float near = 0.1;
    float far  = 100;
    Matrix4 perspectiveMatrix = Matrix4::makeProjectionMatrix(fov, aspectRatio, near,far);

    //perspectiveMatrix.printMat();
    //SDL_Delay(1000);

    Matrix4 viewProjectionMatrix = perspectiveMatrix*viewMatrix;

    //viewProjectionMatrix.printMat();

    //SDL_Delay(1000);

    for (Vector3 f : *faces ){
        //(*vertices)[f.x-1].print();
        Vector3 v1 = viewProjectionMatrix.matMultVec((*vertices)[f.x-1]); //-1 because .obj file starts face count
        //Vector3 v2 = viewProjectionMatrix.matMultVec((*vertices)[f.y-1]); // from 1. Should probably fix this 
        //Vector3 v3 = viewProjectionMatrix.matMultVec((*vertices)[f.z-1]); // At some point
       // v1.print();
        if(v1.x < -1 || v1.x > 1 || v1.y < -1 || v1.y > 1 || v1.z > 1 || v1.z < -1) continue;
        //if(v2.x < -1 || v2.x > 1 || v2.y < -1 || v2.y > 1) continue;
        //v1.print();
        //SDL_Delay(1000);
        drawLine(v1, v1, red);
        //drawLine(v2, v3, green);
        //drawLine(v1, v3, blue);
    }

}   

void Rasterizer::drawLine(Vector3 vertex1, Vector3 vertex2, Uint32 color){

        int x1 = (vertex1.x + 1 ) * mCanvas->mWidth * 0.5;
        int y1 = (-vertex1.y + 1 ) * mCanvas->mHeight * 0.5;
        //int x2 = (vertex2.x +1 ) * mCanvas->mWidth  * 0.5;
        //int y2 = (-vertex2.y +1 ) * mCanvas->mHeight * 0.5;

        setPixelColor(color, x1, y1);

        // //transpose line if it is too steep
        // bool steep = false;
        // if (std::abs(x1-x2) < std::abs(y1-y2) ){
        //     std::swap(x1,y1);
        //     std::swap(x2,y2);
        //     steep = true;
        // }

        // //Redefine line so that it is left to right
        // if ( x1  > x2 ){
        //     std::swap(x1,x2);
        //     std::swap(y1,y2);
        // }
        // int dx = x2 - x1;
        // int dy = y2 - y1;
        // int derror2 = std::abs(dy)*2;
        // int error2 = 0;
        // int y = y1;

        // for(int x=x1; x <= x2 ; x++){
        //     if(steep){
        //             setPixelColor(color, y, x);
        //         }
        //         else{
        //             setPixelColor(color, x, y);
        //         }
        //     error2 += derror2;
        //     if (error2 > dx){
        //         y += (y2 > y1  ? 1 : -1);
        //         error2 -= dx*2;
        //     }

        // }
        
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
