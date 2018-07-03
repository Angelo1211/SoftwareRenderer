#include "engine.h"
#include "windowManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include "vector3.h"
#include "mesh.h"

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

void Engine::loadModels(){
    printf("loading models");
    std::fstream file;
    std::string line, path, v, x ,y ,z;

    Mesh cube;

    path = "../models/cube.obj";
    file.open(path.c_str());

    //Get vertices into mesh
    while(!file.eof()){

        std::getline(file,line);
        std::istringstream iss(line);
        if(line[0] == 'v'){
            iss >> v >> x >> y >> z;
            Vector3 vertex(x,y,z);
            cube.vertices.push_back(vertex);
        }

    }
    file.close();
    int meshSize = cube.vertices.size();
    printf("Meshsize is: %d \n", meshSize);
    for(int i = 0; i < meshSize; ++i){
        Vector3 vertex = cube.vertices[i];
        printf("Vertex  %2.1d: %f, %f, %f \n",i,vertex.x, vertex.y, vertex.z);
    }
}


