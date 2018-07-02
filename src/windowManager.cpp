#include "windowManager.h"

WindowManager::WindowManager(){
    
}

WindowManager::~WindowManager(){
    
}

bool WindowManager::startSDL(){
    bool success = true;
    if( SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Failed to initialize SDL. Error: %s\n", SDL_GetError() );
        success = false;
    }
    return success;
}

bool WindowManager::createWindow(){
    bool success = true;
    mainWindow = SDL_CreateWindow( "SoftwareRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if( mainWindow == nullptr){
            printf("Coult not create window. Error: %s\n", SDL_GetError() );
            success = false;
        }
    return success;
}

bool WindowManager::startUp(){
    if(!startSDL()){
        return false;
    }
    else{
       return createWindow();
    }
}
SDL_Window * WindowManager::getWindow(){
    return mainWindow;
}

void WindowManager::shutDown(){
    SDL_DestroyWindow(mainWindow);
    mainWindow = nullptr;
    SDL_Quit();
}