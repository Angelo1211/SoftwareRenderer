#include "engine.h"
#include <string>
#include <vector3.h>

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
    delete sceneModels;
    sceneModels = nullptr;
    FEInputManager.shutDown();
    FERenderManager.shutDown();
    FEWindowManager.shutDown();

}

void Engine::mainLoop(){
    bool done = false;
    int count = 0;
    printf("Entered Main Loop!\n");
    
    while(!done){
        unsigned int start = SDL_GetTicks();
        ++count;

        //Handle all user input
        done = FEInputManager.processInput();

        //Update entities here in the future
        //Right now only does simple demo stuff
        //Maybe physics based in the future??
        moveModels();

        //Perform all render calculations and update screen
        FERenderManager.render(sceneModels);
        unsigned int end = SDL_GetTicks();
        //SDL_Delay(100);
        printf("%2.1d: Loop elapsed time (ms):%d\n",count,end - start);
    }
}

void Engine::loadModels(){
    //In the future I want to read all o the models in the model folder
    //And build them here.  For now I force it to be only one.
    //Probably should be its own class in the future
    std::string path = "../models/teapot.obj";
    sceneModels = new Model(path);

    //sceneModels->describeMesh();
}


//Engine class moves stuff, for now
//Some kind of physics module should be responsible of this in the future
void Engine::moveModels(){
    float thetax  = 0.001;
    float thetay  = 0.001;
    float thetaz  = 0.00;
    float scale   = 0.999;
    float dd      = 0.0;
    Mesh * modelMesh = sceneModels->getMesh();
    int size = modelMesh->numVertices;
    std::vector<Vector3> *   vertices = &modelMesh->vertices;

    for (int i = 0;i < size; ++i){
        (*vertices)[i].scale(scale);
        (*vertices)[i].rotX(thetax);
        (*vertices)[i].rotY(thetay);
        (*vertices)[i].rotZ(thetaz);
        (*vertices)[i].translate(dd, dd, dd);
    }

}
