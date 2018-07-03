#include "engine.h"

int main(int argc, char *argv[]){

    Engine mainEngine;
    if(mainEngine.startUp()){
        mainEngine.loadModels();
        mainEngine.mainLoop();
    }
    else{
        printf("Engine could not initialize successfully. Shutting down.\n");
    }
    mainEngine.shutDown();

    return 0;
}