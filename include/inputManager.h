#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//Headers
#include "sceneManager.h"
#include "camera.h"
#include "SDL.h"

class InputManager{

    public:
        //Dummy constructors / Destructors
        //Not necessary here, but follow the same format for consistency
        InputManager();
        ~InputManager();

        bool startUp(SceneManager &sceneManager);
        void shutDown();

        //Processes all the SDL events that have ocurred in the given frame
        void processInput(bool &done, unsigned int deltaT);
    private:
        
        SceneManager *sceneController;
        Camera *sceneCamera;
        void handleEvent(SDL_Event * event, bool &done, unsigned int deltaT);
};


#endif