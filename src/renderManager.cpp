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
            if( !createBuffer() ){
                printf("Could not build buffer.\n");
                return false;
            }
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

bool RenderManager::createBuffer(){
    pixelCount = WindowManager::SCREEN_WIDTH * WindowManager::SCREEN_HEIGHT;
    buffer1 = new Uint32[pixelCount];
    SDL_memset(buffer1, 0, pixelCount * sizeof(Uint32) );
    return buffer1 != NULL;
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
    delete[] buffer1;
    buffer1 = nullptr;
    screenTexture.free();
    SDL_DestroyRenderer(mainRenderer);
    mainRenderer = nullptr;
}

void RenderManager::render(){

    //Clear Screen back to black
    clearScreen();

    //Perform any modifications we want on the pixels
    createPixelPattern();

    //Apply the pixel change to the texture
    screenTexture.updateTexture(buffer1);

    //Switch screen texture with back texture and re-draw
    updateScreen();
}

void RenderManager::updateScreen(){
    //Render texture to screen
    screenTexture.renderToScreen(mainRenderer);

    //Update screen to window
    SDL_RenderPresent( mainRenderer );
}

void RenderManager::clearScreen(){
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(mainRenderer);
}

void RenderManager::createPixelPattern(){
    //Get window pixel format
    SDL_PixelFormat *mappingFormat = SDL_AllocFormat (PIXEL_FORMAT);

    //Set color data
    Uint32 red = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0x60);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0x80);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);
    //Color in certain pixels
    for(int i = 0; i < pixelCount; ++i){
        if( (i % 50) == 0){
            buffer1[i] = red;
        }
        if((i % 1000) == 0){
            buffer1[i] = green;
        }
        if((i % 2000) == 0){
            buffer1[i] = blue;
        }
    }
}

