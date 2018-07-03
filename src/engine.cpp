#include "engine.h"
#include "windowManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include <stdio.h>

Engine::Engine(){

}

Engine::~Engine(){
    
}

bool Engine::startUp(){
    bool success = true;
    //Startup window manager and create window
    if( !FEWindowManager.startUp() ){
        success = false;
        printf("Failed to initialize window manager.\n");
    }
    else{
        if( !FERenderManager.startUp(FEWindowManager) ){
            success = false;
            printf("Failed to initialize render manager.\n");
        }
        else{
            if( !FEInputManager.startUp() ){
            success = false;
            printf("Failed to initialize input manager.\n");
            }
        }
    }
    return success;
}

void Engine::shutDown(){
    FEInputManager.shutDown();
    FERenderManager.shutDown();
    FEWindowManager.shutDown();
}

void Engine::mainLoop(){
    bool done = false;
    int count = 0;
    printf("Entered Main Loop!\n");
    
    while(!done){
        ++count;

        //Handle all user input
        done = FEInputManager.processInput();

        //Update entities here in the future
        //TO DO

        //Perform all render calculations and update screen
        FERenderManager.render();

        SDL_Delay(500);
        printf("Loop Iteration N:%d\n",count);
    }
}
