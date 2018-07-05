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
            return true;
        }
    }
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
                            new Uint32[pixelCount]);
    SDL_memset(mainCanvas->mBuffer, 0, mainCanvas->mPixelCount * sizeof(Uint32) );
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

void RenderManager::render(Model *models){

    //Clear Screen back to black
    clearScreen();
    
    //Perform any modifications we want on the pixels
    raster->drawModels(models);

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
}

