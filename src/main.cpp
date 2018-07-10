#include "engine.h"

int main(int argc, char *argv[]){

    Engine SSGE; //Simple Software Graphics Engine
    if(SSGE.startUp()){
        SSGE.loadModels();
        SSGE.mainLoop();
    }
    else{
        printf("SSGE could not initialize successfully. Shutting down.\n");
    }
    SSGE.shutDown();

    return 0;
}