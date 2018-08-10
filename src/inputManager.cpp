#include "inputManager.h"

InputManager::InputManager(){}
InputManager::~InputManager(){}

bool InputManager::startUp(SceneManager &sceneManager){
    sceneController = &sceneManager;
    sceneCamera = (sceneController->getCurrentScene()->getCurrentCamera());

    return true;
}

void InputManager::shutDown(){
    //Nothing to do yet
}

void InputManager::processInput(bool &done, unsigned int deltaT){
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
            handleEvent(&event, done, deltaT);
        }
    }
}

//Handles user keyboard input
//TODO: handle mouse input 
void InputManager::handleEvent(SDL_Event * event, bool &done, unsigned int deltaT){
    float speed = sceneCamera->camSpeed;// * deltaT;
    //Handling keyboard input
    if( event->type == SDL_KEYDOWN ){
        //keys 1-5 switch to different scene
        //WASD control strafe movement
        std::string sceneID = "0";
        switch( event->key.keysym.sym )
        {   
            //SCENE CODE
            case SDLK_1:
            sceneID = "teapot";
            break;

            case SDLK_2:
            sceneID = "firehydrant";
            break;

            case SDLK_3:
            sceneID = "firehydrant";
            break;

            case SDLK_4:
            sceneID = "firehydrant";
            break;

            case SDLK_5:
            sceneID = "firehydrant";
            break;

            //WINDOW CONTROL OPTIONS
            case SDLK_ESCAPE:
            done = true;
            return; 

            //MOVEMENT CONTROLS (STRAFING)
            case SDLK_w:
            if(sceneCamera->orbiting){
                sceneCamera->radius += sceneCamera->radius * speed;
            }
            else{
                sceneCamera->position += sceneCamera->front * speed;
            }
            break;

            case SDLK_s:
            if(sceneCamera->orbiting){
                sceneCamera->radius -= sceneCamera->radius * speed;
            }
            else{
                sceneCamera->position -= sceneCamera->front * speed;
            }
            break;

            case SDLK_a:
            sceneCamera->position -= sceneCamera->side * speed;
            break;

            case SDLK_d:
            sceneCamera->position += sceneCamera->side * speed;
            break;

            //CAMERA CONTROLS (RESET AND ORBITING)
            case SDLK_r:
            sceneCamera->position = Vector3f(0, 0, 8.0);
            sceneCamera->target.zero();  
            sceneCamera->radius = 3;  
            break;

            case SDLK_TAB:
            sceneCamera->orbiting = !sceneCamera->orbiting;
            sceneCamera->position = Vector3f(0, 0, 8.0);
            sceneCamera->target.zero();
            sceneCamera->radius = 3;    
            break;

            default:
            break;

        }

        //Only switch scene if a scene-key (1-5) was pressed
        //Exit if the scene could not be loaded for some reason
        if ( sceneID != "0" ){
            if( !sceneController->switchScene(sceneID) ){
                printf("Failed to switch scene! Quitting.\n");
                done = true;
                return;
            }
            else{
                printf("Loaded %s Scene.\n", sceneID.c_str());
                sceneCamera = (sceneController->getCurrentScene()->getCurrentCamera());
                sceneCamera->position = Vector3f(0, 0, 8.0);
                sceneCamera->target.zero();
            }

        }
    }
}