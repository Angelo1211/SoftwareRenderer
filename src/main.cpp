// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-02
// PURPOSE      : Program initialization and shutdown
// ===============================

#include "engine.h"

int main(int argc, char *argv[]){

    Engine SSGE; //"Simple" Software Graphics Engine
    if(SSGE.startUp()){
        SSGE.run();
    }
    else{
        printf("SSGE could not initialize successfully. Shutting down.\n");
    }
    SSGE.shutDown();

    return 0;
}