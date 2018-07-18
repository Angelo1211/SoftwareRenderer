#include "inputManager.h"

InputManager::InputManager(){}
InputManager::~InputManager(){}

bool InputManager::startUp(){
    //Nothing to do yet
    return true;
}

void InputManager::shutDown(){
    //Nothing to do yet
}

bool InputManager::processInput(){
    bool done = false;
    SDL_Event Event;
    while(SDL_PollEvent(&Event)){
       done = handleEvent(&Event);
    }
    return done;
}

//TODO all input handling will go here
bool InputManager::handleEvent(SDL_Event * event){
    if(event->type == SDL_QUIT){
        return true;
    }
    return false;
}