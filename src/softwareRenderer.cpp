#include "softwareRenderer.h"

SoftwareRenderer::SoftwareRenderer(){

}

SoftwareRenderer::~SoftwareRenderer(){

}

bool SoftwareRenderer::startUp(DisplayManager &displayManager){
    screen = &displayManager;
    if( !createBuffers() ){
        return false;
    }
    //Create rasterizer to begin drawing
    raster = new Rasterizer(pixelBuffer);
    //createProjectionMatrix();
    return true;
}

void SoftwareRenderer::shutDown(){
    delete zBuffer;
    delete pixelBuffer;
    delete raster;
}

void SoftwareRenderer::clearBuffers(){
    zBuffer->clear();
    pixelBuffer->clear();
}

bool SoftwareRenderer::createBuffers(){
    int w = DisplayManager::SCREEN_WIDTH;
    int h = DisplayManager::SCREEN_HEIGHT;
    int pixelCount = w*h;
    bool success = true;

    zBuffer = new Buffer<float>(w, h, new float[pixelCount]);
    if( zBuffer == nullptr){
        printf("Could not build z-Buffer.\n");
        success = false;
    }
    else{
        pixelBuffer = new Buffer<Uint32>(w, h, new Uint32[pixelCount]);
        if( pixelBuffer == nullptr){
            printf("Could not build pixel Buffer.\n");
            success = false;
        }
    }
    return success;
}

void SoftwareRenderer::render(){

    //Clear screen and reset current buffers
    screen->clear();
    clearBuffers();

    //Getting the meshes and faces 
    // Mesh * modelMesh = models->getMesh();
    // std::vector<Vector3> * faces = &modelMesh->faces;
    // std::vector<Vector3> * vertices = &modelMesh->vertices;

    // //If the frustrum culling returns true it means the model has been culled
    // //Because no other models are in the scene we return
    // // if (frustrumCulling(models, viewMatrix)){
    // //     printf("Model culled!\n");
    // //     return;
    // // } 
    // //Kind of a vertex shader I guess?
    // for (Vector3 f : *faces ){

    //     //View matrix transform
    //     Vector3 v0 = viewMatrix.matMultVec((*vertices)[f.x-1]); //-1 because .obj file starts face count
    //     Vector3 v1 = viewMatrix.matMultVec((*vertices)[f.y-1]); // from 1. Should probably fix this 
    //     Vector3 v2 = viewMatrix.matMultVec((*vertices)[f.z-1]); // At some point

    //     //Back face culling in view space
    //     Vector3 N1 = v1 - v0;
    //     Vector3 N2 = v2 - v0;
    //     Vector3 N  = (N1.crossProduct(N2)).normalized();
    //     Vector3 neg = (v0.neg()).normalized();
    //     float intensity = N.dotProduct(neg);
    //     if(intensity < 0.0) continue; //Exit if you can't even see it


    //     //stupid frustrum culling
    //     //if(v1.x < -1 || v1.x > 1 || v1.y < -1 || v1.y > 1 || v1.z > 1 || v1.z < -1) continue;
    //     //if(v2.x < -1 || v2.x > 1 || v2.y < -1 || v2.y > 1 || v2.z > 1 || v2.z < -1) continue;
    //     //if(v3.x < -1 || v3.x > 1 || v3.y < -1 || v3.y > 1 || v3.z > 1 || v3.z < -1) continue;

    //     // Projection matrix transformation
    //     v0 = projectionMatrix.matMultVec(v0);
    //     v1 = projectionMatrix.matMultVec(v1);
    //     v2 = projectionMatrix.matMultVec(v2);

    //     //Rasterizing and shading in one
    raster->makeCoolPattern();
               
    // }
    //Apply the pixel changes and redraw
    screen->draw(pixelBuffer);
}

// bool SoftwareRenderer::frustrumCulling(Model *model, Matrix4 &viewMatrix){
//     bool cull = false;
//     Matrix4 viewProjectionMat = projectionMatrix*viewMatrix;
//     return cull;
// }

// bool SoftwareRenderer::createCanvas(){
//     int pixelCount = WindowManager::SCREEN_WIDTH * WindowManager::SCREEN_HEIGHT;
//     int pitch      = WindowManager::SCREEN_WIDTH * sizeof(Uint32);
//     mainCanvas = new Canvas(WindowManager::SCREEN_WIDTH,
//                             WindowManager::SCREEN_HEIGHT,
//                             pixelCount, pitch,
//                             new Uint32[pixelCount], new float[pixelCount]);
//     SDL_memset(mainCanvas->mBuffer, 0, mainCanvas->mPixelCount * sizeof(Uint32) );
//     for(int i = 0; i < mainCanvas->mPixelCount;++i){
//         mainCanvas->mDBuffer[i]  = 0.0f;
//     }
//     return mainCanvas != NULL;
// }