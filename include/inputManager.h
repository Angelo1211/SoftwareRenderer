#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//Headers
#include"SDL.h"

class InputManager{

    public:
        //Dummy constructors / Destructors
        //Not necessary here, but follow the same format for consistency
        InputManager();
        ~InputManager();

        bool startUp();
        void shutDown();

        //Processes all the SDL events that have ocurred in the given frame
        //For now only listens to the SDL_QUIT event that signifies the window was 
        //closed.
        bool processInput();
    private:
        bool handleEvent(SDL_Event * event);
};


#endif