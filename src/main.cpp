#include "engine.h"

//Global variables
// SDL_Texture *gTexture   = nullptr;
// Uint32 *gBuffer         = nullptr;
// bool quitFlag           = false;

int main(int argc, char *argv[]){

    Engine mainEngine;
    if(mainEngine.startUp()){
        mainEngine.mainLoop();
    }
    else{
        printf("Engine could not initialize successfully. Shutting down.\n");
    }
    mainEngine.shutDown();

    return 0;
}