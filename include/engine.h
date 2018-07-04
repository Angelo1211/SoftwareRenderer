#ifndef ENGINE_H
#define ENGINE_H

//Headers
#include "windowManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include "model.h"

class Engine{

    public:
        Engine();

        ~Engine();

        bool startUp();

        void shutDown();

        void mainLoop();

        void loadModels();

        void moveModels();

    private:
        WindowManager FEWindowManager;
        RenderManager FERenderManager;
        InputManager  FEInputManager;
        Model        *sceneModels;
};

#endif