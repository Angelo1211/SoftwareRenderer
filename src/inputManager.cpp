#include "inputManager.h"

InputManager::InputManager(){}
InputManager::~InputManager(){}

bool InputManager::startUp(SceneManager &sceneManager){
    sceneController = &sceneManager;
    sceneCamera = (sceneController->getCurrentScene()->getCurrentCamera());
    
    //Only really care about relative mouse motion because we're building a space camera
    bool success = !SDL_SetRelativeMouseMode(SDL_TRUE);
    return success;
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
            sceneID = "teapotMetal";
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
            sceneCamera->resetCamera(); 
            break;

            case SDLK_TAB:
            sceneCamera->orbiting = !sceneCamera->orbiting;
            sceneCamera->resetCamera();   
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
                sceneCamera->resetCamera(); 
            }

        }
    }
    //Handling Mouse Motion
    else if( event->type == SDL_MOUSEMOTION){
        //Only move camera if the right button is pressed
        if( event->motion.state & SDL_BUTTON_RMASK ) {
            float sens = 0.05f;
            float xOffset = (float)event->motion.xrel * sens;
            float yOffset = -(float)event->motion.yrel * sens;

            sceneCamera->yaw   += xOffset;
            sceneCamera->pitch += yOffset;

            //Limiting the range of the pitch to avoid flips
            if(sceneCamera->pitch > 89.0f){
                sceneCamera->pitch =  89.0f;
            }
            else if(sceneCamera->pitch < -89.0f){
                sceneCamera->pitch = -89.0f;
            }

            //Updating the front and side vectors to allow wasd movement and 
            //free camera movement.
            sceneCamera->front.x = cos( sceneCamera->pitch * M_PI / 180.0f ) * cos( sceneCamera->yaw * M_PI / 180.0f );
            sceneCamera->front.y = sin( sceneCamera->pitch * M_PI / 180.0f );
            sceneCamera->front.z = cos( sceneCamera->pitch * M_PI / 180.0f ) * sin( sceneCamera->yaw * M_PI / 180.0f );
            sceneCamera->front   = sceneCamera->front.normalized();
            sceneCamera->side    = sceneCamera->front.crossProduct(sceneCamera->up);
        }
    }
    else if( event->type == SDL_MOUSEWHEEL){
        float zoom = 5.0f;
        float fov  = sceneCamera->cameraFrustrum.fov; 
        if(event->wheel.y > 0){ // scroll up
            fov -= zoom;
        }
        else if(event->wheel.y < 0) {// scroll down
            fov += zoom;
        }

        if(fov < 30){
            fov = 20;
        }
        else if (fov > 120){
            fov = 120;
        }

        sceneCamera->cameraFrustrum.fov = fov;
    }
}