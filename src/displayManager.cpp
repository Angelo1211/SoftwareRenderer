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
            if( !createSDLRenderer() ){
                success = false;
            }
            else{
                if( !createScreenTexture() ){
                    success = false;
                }
            }

        }
    }
    return success;
}

void DisplayManager::shutDown(){
    mTexturePixels = nullptr;
  
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;

    SDL_DestroyRenderer(mSDLRenderer);
    mSDLRenderer = nullptr;

    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

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
    mWindow = SDL_CreateWindow( "SoftwareRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if( mWindow == nullptr){
        printf("Could not create window. Error: %s\n", SDL_GetError() );
        return false;
    }
    return true;
}

bool DisplayManager::createSDLRenderer(){
    mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if(mSDLRenderer == nullptr){
        printf("Could not create renderer context. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool DisplayManager::createScreenTexture(){
    mTexture = SDL_CreateTexture(mSDLRenderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(mTexture == NULL){
        printf("Could not create texture. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void DisplayManager::clear(){
    SDL_SetRenderDrawColor(mSDLRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(mSDLRenderer);
}

void DisplayManager::swapBuffers(Buffer<Uint32> *pixels){
    
    //Lock texture for manipulation
    SDL_LockTexture(mTexture, NULL, &mTexturePixels, &mTexturePitch );
    
    //Copy pixels to texture memory PRETTY SLOW. FIX?
    memcpy(mTexturePixels, pixels->buffer, SCREEN_HEIGHT*mTexturePitch);

    //Unlocking texture to apply pixel changes
    SDL_UnlockTexture(mTexture);
    mTexturePixels = nullptr;

    //Setting blendmode for copy operation to renderer backbuffer
    SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(mSDLRenderer, mTexture, NULL , NULL);

    //Update screen to window
    SDL_RenderPresent( mSDLRenderer );
}