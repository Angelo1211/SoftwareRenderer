#ifndef ENGINE_H
#define ENGINE_H

//Headers
#include "displayManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include "sceneManager.h"

//Minimal graphics engine application
class Engine{

    public:
        //Dummy constructors / Destructors
        Engine();
        ~Engine();

        //I use these methods instead of constructors and destructors
        //because I want to be able to control initialization order. 
        //You'll see the same idea applied to all subsystem level classes.
        bool startUp();
        void shutDown();

        //Contains the scene switching logic and the main application loop
        void run();

    private:
        DisplayManager   gDisplayManager;
        RenderManager    gRenderManager;
        InputManager     gInputManager;
        SceneManager     gSceneManager;
};

#endif