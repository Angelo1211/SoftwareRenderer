#include "displayManager.h"

//Dummy constructurs/destructors
DisplayManager::DisplayManager(){}
DisplayManager::~DisplayManager(){}

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
            if( !createScreenSurface() ){
                success = false;
            }
        }
    }
    return success;
}

void DisplayManager::shutDown(){

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

bool DisplayManager::createScreenSurface(){
    mSurface = SDL_GetWindowSurface(mWindow);
    if(mSurface == NULL){
        printf("Could not create window surface. Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void DisplayManager::clear(){
    //Clear to black
    SDL_LockSurface(mSurface);
    SDL_memset(mSurface->pixels,0 ,mSurface->h * mSurface->pitch);
    SDL_UnlockSurface(mSurface);
}

void DisplayManager::swapBuffers(Buffer<Uint32> *pixels){
    //Allows surface editing 
    SDL_LockSurface(mSurface);

    //Copy pixels buffer resuls to screen surface
    memcpy(mSurface->pixels, pixels->buffer, pixels->mHeight*pixels->mPitch);
    SDL_UnlockSurface(mSurface);

    //Update surface to window
    SDL_UpdateWindowSurface(mWindow);

}