#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-02
// PURPOSE      : Managing user input either via keyboard, mouse movement, clicking, or
//                even mouse wheel. It's currently mostly used for camera traversal,
//                scene switching and exiting the program.
// ===============================
// SPECIAL NOTES: It works by directly calling functions that perform these actions,
// that is it doesn't tell the camera to move by x amount on the next update cycle.
// The input manager goes ahead and does just that. And withing the update calls of each
// object that is affected it deals with the consequences of performing that action.
// ===============================

//Headers
#include "sceneManager.h"
#include "camera.h"
#include "SDL.h"

class InputManager{
    public:
        //Dummy constructors / Destructors
        //Not really necessary here, but follows the same format for consistency
        InputManager();
        ~InputManager();

        bool startUp(SceneManager &sceneManager);
        void shutDown();

        //Processes all the SDL events that have ocurred in the past frame
        void processInput(bool &done, unsigned int deltaT);
    private:
        SceneManager *sceneController;
        Camera *sceneCamera;
        
        //Where specific events are handled
        void handleEvent(SDL_Event * event, bool &done, unsigned int deltaT);
};


#endif