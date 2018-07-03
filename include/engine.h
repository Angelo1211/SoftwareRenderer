#ifndef ENGINE_H
#define ENGINE_H

//Headers
#include "windowManager.h"
#include "renderManager.h"
#include "inputManager.h"

class Engine{

    public:
        Engine();

        ~Engine();

        bool startUp();

        void shutDown();

        void mainLoop();

        void loadModels();

    private:
        WindowManager FEWindowManager;
        RenderManager FERenderManager;
        InputManager  FEInputManager;
};


#endif