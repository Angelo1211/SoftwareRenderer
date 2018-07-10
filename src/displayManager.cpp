#include "displayManager.h"

DisplayManager::DisplayManager(){
    
}

DisplayManager::~DisplayManager(){
    
}

bool DisplayManager::startUp(){
    bool success = true;
    if( !startSDL() ){
        success = false;
    }
    else{
        if( !createWindow() ){
            success = false;
        }
        else{
            // if( !createSDLRenderer() ){
            //     success = false;
            // }
            // else{
            //     if( !createScreenTexture() ){
            //         success = false;
            //     }
            // }

        }
    }
    return success;
}

void DisplayManager::shutDown(){
    // screenTexture.free();
  
    // SDL_FreeSurface(surface);

    // SDL_DestroyRenderer(SDLRenderer);
    // SDLRenderer = nullptr;

    SDL_DestroyWindow(mainWindow);
    mainWindow = nullptr;

    SDL_Quit();
}

bool DisplayManager::startSDL(){
    if( SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError() );
        return  false;
    }
    return true;
}

bool DisplayManager::createWindow(){
    mainWindow = SDL_CreateWindow( "SoftwareRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    surface = SDL_GetWindowSurface(mainWindow);
    if( mainWindow == nullptr){
        printf("Could not create window. Error: %s\n", SDL_GetError() );
        return false;
    }
    return true;
}

bool DisplayManager::createSDLRenderer(){
    //SDLRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    SDLRenderer = SDL_CreateSoftwareRenderer(surface);
    if(SDLRenderer == nullptr){
        printf("Could not create renderer context. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool DisplayManager::createScreenTexture(){
    if(! screenTexture.createBlank(SDLRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)){
        printf("Could not create texture. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void DisplayManager::clear(){
    SDL_LockSurface(surface);
    SDL_memset(surface->pixels,0 ,surface->h * surface->pitch);
    SDL_UnlockSurface(surface);
    //SDL_SetRenderDrawColor(SDLRenderer, 0x00, 0x00, 0x00, 0xFF);
    //SDL_RenderClear(SDLRenderer);

}

void DisplayManager::draw(Buffer<Uint32> *pixels){
    
    // screenTexture.update(pixels->buffer);

    // //Render texture to screen
    // screenTexture.renderToScreen(SDLRenderer);

    // //Update screen to window
    // SDL_RenderPresent( SDLRenderer );

    //Testing pure software rendering
    SDL_LockSurface(surface);
    memcpy(surface->pixels, pixels->buffer, pixels->mHeight*pixels->mPitch);
    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(mainWindow);
}