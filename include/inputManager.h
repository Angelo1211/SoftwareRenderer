#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

//Headers
#include"SDL.h"

class InputManager{

    public:
        InputManager();

        ~InputManager();

        bool startUp();

        bool processInput();

        void shutDown();

    private:
        bool handleEvent(SDL_Event * event);

};


#endif