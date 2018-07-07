#include "engine.h"
#include <string>
#include <vector3.h>
#include <matrix.h>
#include <math.h>

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
    unsigned int end = 0;
    unsigned int start = 0;
    printf("Entered Main Loop!\n");
    
    while(!done){
        start = SDL_GetTicks();
        ++count;

        //Handle all user input
        done = FEInputManager.processInput();

        //Update entities here in the future
        //Right now only does simple demo stuff
        //Maybe physics based in the future??
        updateCamera();

        //Perform all render calculations and update screen
        FERenderManager.render(sceneModels, viewMatrix);
        end = SDL_GetTicks();
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

    //We also initialize the model position here position here
    TransformParameters initParameters;
    //initParameters.scaling = Vector3(1, 60, 60);
    initParameters.rotation = Vector3(0,0,0);
    initParameters.translation = Vector3(0, -1, 0);

    sceneModels->initPosition(initParameters);

    //sceneModels->describeMesh();
}

//This should be its own class in the future
void Engine::updateCamera(){
    float t = static_cast<float>(SDL_GetTicks());
    float radius = 6;
    float camX   = std::sin(t/4000) * radius;
    float camZ   = std::cos(t/4000) * radius;
    Vector3 pos(camX, 0, camZ);
    Vector3 tar;
    Vector3 v(0,1,0);
    viewMatrix = Matrix4::lookAt(pos,tar,v);
    //viewMatrix = (Matrix4::makeTranslateMat(0,camX*0.25,0)*viewMatrix);
}