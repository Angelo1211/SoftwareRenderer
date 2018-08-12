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
            sceneCamera->position = Vector3f(0, 0, 8.0);
            sceneCamera->target.zero();  
            sceneCamera->side = sceneCamera->front.crossProduct(sceneCamera->up);
            sceneCamera->front = Vector3f(0, 0, -1);
            sceneCamera->radius = 2;  
            break;

            case SDLK_TAB:
            sceneCamera->orbiting = !sceneCamera->orbiting;
            sceneCamera->position = Vector3f(0, 0, 8.0);
            sceneCamera->target.zero();
            sceneCamera->side = sceneCamera->front.crossProduct(sceneCamera->up);
            sceneCamera->front = Vector3f(0, 0, -1);
            sceneCamera->radius = 2;    
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
                sceneCamera->side = sceneCamera->front.crossProduct(sceneCamera->up);
                sceneCamera->front = Vector3f(0, 0, -1);
                
            }

        }
    }
    //Handling Mouse Motion
    else if( event->type == SDL_MOUSEMOTION){
        float sens = 0.001f;
        float xRot = -(float)event->motion.yrel * sens;
        float yRot = -(float)event->motion.xrel * sens;

        Matrix4 cameraRot = Matrix4::fullRotMat(xRot, yRot, 0.0f);

        sceneCamera->front = cameraRot.matMultVec(sceneCamera->front).normalized();
        sceneCamera->side  = cameraRot.matMultVec(sceneCamera->side).normalized();
        
        // Matrix4 camTransform = (sceneCamera->viewMatrix);
        // Vector3f newDir      = Vector3f((float)event->motion.xrel*0.01f, -(float)event->motion.yrel*0.01f, 0.0f);
        // Vector3f relMov      = camTransform.matMultDir(newDir);

        // sceneCamera->front = (sceneCamera->front + relMov).normalized();
        
        // if(event->motion.xrel > 0){
        //     sceneCamera->front = (sceneCamera->front + Vector3f((float)event->motion.xrel*0.001f, 0.0f, 0.0f)).normalized();
        //     printf("Moved right!\n");
            
        // }

        // if(event->motion.xrel < 0){
            
        //     printf("Moved left!\n");
        // }

        // if(event->motion.yrel > 0){
        //     printf("Moved down!\n");
        // }

        // if(event->motion.yrel < 0){
        //     printf("Moved up!\n");
        // }
    }
}


//Handling Mouse Input
    // else if((event->type == SDL_MOUSEBUTTONDOWN) && (event->button.button == SDL_BUTTON_RIGHT)){
    //     printf("Right mouse Click detected\n");
    //     SDL_Delay(1000);
    // }