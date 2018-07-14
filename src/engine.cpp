#include "engine.h"

Engine::Engine(){
}

Engine::~Engine(){
}

//Starts up subsystems in an order that satifies their dependencies
bool Engine::startUp(){
    bool success = true;
    //Start up of all SDL related content
    if( !gDisplayManager.startUp() ){
        success = false;
        printf("Failed to initialize window manager.\n");
    }
    else{
        //Loads default scene
        if( !gSceneManager.startUp() ){
            success = false;
            printf("Failed to initialize scene manager.\n");
        }
        else{
            //Initializes renderer and connects it to the window manager
            //Also gets pointer to current scene
            if( !gRenderManager.startUp(gDisplayManager,gSceneManager) ){
            success = false;
            printf("Failed to initialize render manager.\n");
            
            }
            else{
                if ( !gInputManager.startUp() ){
                    success = false;
                    printf("Failed to initialize input manager.\n");
                }
            }
        }
    }
    return success;
}

//Closing in opposite order to avoid dangling pointers
void Engine::shutDown(){
    gInputManager.shutDown();
    printf("Closed input manager.\n");

    gRenderManager.shutDown();
    printf("Closed renderer manager.\n");
    
    gSceneManager.shutDown();
    printf("Closed Scene manager.\n");
    
    gDisplayManager.shutDown();
    printf("Closed display manager.\n");
}

//Runs main application loop and allows for scene changes
void Engine::run(){

    //Main flags
    bool done = false;
    bool switchScene = false;

    //Counters
    int count = 0;
    unsigned int end = 0;
    unsigned int start = 0;

    printf("Entered Main Loop!\n");
    while(!done){
        start = SDL_GetTicks();
        ++count;
        
        //If scene switching has been called you break out of the current loop 
        if( switchScene ){
            if( !gSceneManager.switchScene() ){
                printf("Failed to switch scene! Quitting.\n");
                break;
            }
            else switchScene = false;
        }

        //Handle all user input
        done = gInputManager.processInput();

        //Update all models and camera in the current scene
        gSceneManager.update();

        //Update Rendering Queue and draw each item 
        gRenderManager.render();

        end = SDL_GetTicks();
        printf("%2.1d: Loop elapsed time (ms):%d\n",count,end - start);
        //break;
    }
    printf("Closing engine\n");
}
