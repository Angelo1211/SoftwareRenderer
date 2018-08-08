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
            handleEvent(&event);
        }
    }
}

//TODO all input handling will go here
void InputManager::handleEvent(SDL_Event * event){

}