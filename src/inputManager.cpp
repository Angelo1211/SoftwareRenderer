#include "inputManager.h"

InputManager::InputManager(){}
InputManager::~InputManager(){}

bool InputManager::startUp(SceneManager &sceneManager){

    sceneController = &sceneManager;
    return true;
}

void InputManager::shutDown(){
    //Nothing to do yet
}

void InputManager::processInput(bool &done){
    SDL_Event event;
    while(SDL_PollEvent(&event)){

        //First check if user requests an exit
        if(event.type == SDL_QUIT){
            done = true;
            return;       
        }
        //Handle any other relevant input data 
        //Keypresses, mouse etc
        else{
            handleEvent(&event, done);
        }
    }
}

//Handles user keyboard input
//TODO: handle mouse input 
void InputManager::handleEvent(SDL_Event * event, bool &done ){

    //Handling keyboard input
    if( event->type == SDL_KEYDOWN ){
        std::string sceneID = "0";
        //Switch scenes based on keypresses
        switch( event->key.keysym.sym )
        {
            case SDLK_1:
            sceneID = "teapot";
            break;

            case SDLK_2:
            sceneID = "firehydrant";
            break;
        }

        if ( sceneID != "0" ){

            if( !sceneController->switchScene(sceneID) ){
                printf("Failed to switch scene! Quitting.\n");
                done = true;
                return;
            }
            else{
                printf("Loaded %s Scene.\n", sceneID.c_str());
            }

        }
    }
}