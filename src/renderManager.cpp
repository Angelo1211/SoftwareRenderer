#include "renderManager.h"

RenderManager::RenderManager(){

}

RenderManager::~RenderManager(){

}

bool RenderManager::startUp(WindowManager WindowManager){
    //Get our current window and create a renderer for it.
    SDL_Window * mainWindow = WindowManager.getWindow();
    if( !createRenderer(mainWindow) ){
        return false;
    }
    else{
        if( !createScreenTexture() ){
            return false;
        }
        else{
            if( !createCanvas() ){
                printf("Could not build canvas.\n");
                return false;
            }
            //Create rasterizer to begin drawing
            raster = new Rasterizer(mainCanvas);
            createProjectionMatrix();
            return true;
        }
    }
}

void RenderManager::createProjectionMatrix(){
    float fov = 75;
    float aspectRatio = mainCanvas->mWidth / (float)mainCanvas->mHeight;
    float near = 1;
    float far  = 100;
    projectionMatrix = Matrix4::makeProjectionMatrix(fov, aspectRatio, near,far);
}

bool RenderManager::createScreenTexture(){
    if(! screenTexture.createBlank(mainRenderer, WindowManager::SCREEN_WIDTH, WindowManager::SCREEN_HEIGHT)){
        printf("Could not create texture. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool RenderManager::createCanvas(){
    int pixelCount = WindowManager::SCREEN_WIDTH * WindowManager::SCREEN_HEIGHT;
    int pitch      = WindowManager::SCREEN_WIDTH * sizeof(Uint32);
    mainCanvas = new Canvas(WindowManager::SCREEN_WIDTH,
                            WindowManager::SCREEN_HEIGHT,
                            pixelCount, pitch,
                            new Uint32[pixelCount], new float[pixelCount]);
    SDL_memset(mainCanvas->mBuffer, 0, mainCanvas->mPixelCount * sizeof(Uint32) );
    for(int i = 0; i < mainCanvas->mPixelCount;++i){
        mainCanvas->mDBuffer[i]  = 0.0f;
    }
    //printf("I:%p, %p | P:%p, %p\n",mainCanvas->mBuffer,mainCanvas->mBuffer + mainCanvas->mPixelCount -1, mainCanvas->mDBuffer,mainCanvas->mDBuffer + mainCanvas->mPixelCount -1);
    //SDL_Delay(3000);
    return mainCanvas != NULL;
}

bool RenderManager::createRenderer(SDL_Window *window){
    mainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(mainRenderer == nullptr){
        printf("Could not create renderer context. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void RenderManager::shutDown(){
    delete raster;
    delete mainCanvas->mBuffer;
    delete mainCanvas;
    screenTexture.free();
    SDL_DestroyRenderer(mainRenderer);
    mainRenderer = nullptr;
}

void RenderManager::render(Model *models, Matrix4 &viewMatrix){

    //Clear Screen back to black
    clearScreen();

    //Combination of both matrices
    Matrix4 viewProjectionMatrix = projectionMatrix*viewMatrix;
    Vector3 forward(viewMatrix(2,0),viewMatrix(2,1),viewMatrix(2,2));

    //Getting the meshes and faces 
    Mesh * modelMesh = models->getMesh();
    std::vector<Vector3> * faces = &modelMesh->faces;
    std::vector<Vector3> * vertices = &modelMesh->vertices;

    //Kind of a vertex shader I guess?
    //printf("Starting per face operations..\n");
    for (Vector3 f : *faces ){
        Vector3 v1 = viewProjectionMatrix.matMultVec((*vertices)[f.x-1]); //-1 because .obj file starts face count
        Vector3 v2 = viewProjectionMatrix.matMultVec((*vertices)[f.y-1]); // from 1. Should probably fix this 
        Vector3 v3 = viewProjectionMatrix.matMultVec((*vertices)[f.z-1]); // At some point
        //if(v1.x < -1 || v1.x > 1 || v1.y < -1 || v1.y > 1 || v1.z > 1 || v1.z < -1) continue;
        //if(v2.x < -1 || v2.x > 1 || v2.y < -1 || v2.y > 1 || v2.z > 1 || v2.z < -1) continue;
        //if(v3.x < -1 || v3.x > 1 || v3.y < -1 || v3.y > 1 || v3.z > 1 || v3.z < -1) continue;
       
        //Back face culling in world space
        Vector3 N1 = (*vertices)[f.y-1] - (*vertices)[f.x-1];
        Vector3 N2 = (*vertices)[f.z-1] - (*vertices)[f.x-1];
        Vector3 N  = N1.crossProduct(N2);
        N = N.normalized();
        float intensity = N.dotProduct(forward);
        if(intensity > 0.0){
            raster->drawTriangles(v1,v2,v3,intensity);   
        }
    }
    //Apply the pixel changes and redraw
    updateScreen();
}

void RenderManager::updateScreen(){
    
    screenTexture.updateTexture(mainCanvas->mBuffer);

    //Render texture to screen
    screenTexture.renderToScreen(mainRenderer);

    //Update screen to window
    SDL_RenderPresent( mainRenderer );
}

void RenderManager::clearScreen(){
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(mainRenderer);
    memset(mainCanvas->mBuffer,0, mainCanvas->mPitch*mainCanvas->mHeight);
    for(int i = 0; i < mainCanvas->mPixelCount;++i){
        mainCanvas->mDBuffer[i]  = 0.0f;
    }
}

