#ifndef ENGINE_H
#define ENGINE_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-02
// PURPOSE      : Application class containing all high level logic and init / shutdown
//                routines for each major subsystem. The purpose of this program is to
//                to build a functioning graphics engine without using libraries
//                such as OpenGL or DirectX.
// ===============================
// SPECIAL NOTES: Built for educational purposes only.
// ===============================

//Headers
#include "displayManager.h"
#include "renderManager.h"
#include "inputManager.h"
#include "sceneManager.h"

//Very basic graphics engine application. 
//In charge of initializing and closing down all manager-level classes in a safe way.
class Engine
{
  public:
    //Dummy constructors / Destructors
    Engine();
    ~Engine();

    //I use these methods instead of constructors and destructors
    //because I want to be able to control initialization order.
    //You'll see the same idea applied to all manager level classes.
    bool startUp();
    void shutDown();

    //Contains all high level logic and the main application loop
    void run();

  private:
    DisplayManager gDisplayManager;
    RenderManager gRenderManager;
    InputManager gInputManager;
    SceneManager gSceneManager;
};

#endif