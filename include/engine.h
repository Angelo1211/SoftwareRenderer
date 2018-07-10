#ifndef ENGINE_H
#define ENGINE_H

//Headers
#include "windowManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include "model.h"
#include "matrix.h"

class Engine{

    public:
        Engine();

        ~Engine();

        bool startUp();

        void shutDown();

        void mainLoop();

        void loadModels();

        //Will not be here forever
        void updateCamera();

    private:
        WindowManager FEWindowManager;
        RenderManager FERenderManager;
        InputManager  FEInputManager;
        Model        *sceneModels;
        Matrix4      viewMatrix;
};

#endif