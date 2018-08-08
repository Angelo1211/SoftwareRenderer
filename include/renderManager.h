#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "displayManager.h"
#include "sceneManager.h"
#include "softwareRenderer.h"
#include "model.h"
#include <queue>

//High level render operations that shouldn't be done by the
//basic graphics lib.
class RenderManager{

    public:
        //Dummy constructors / Destructors
        RenderManager();
        ~RenderManager();

        //Gets scene and display info. Will be used to build render Queue
        bool startUp(DisplayManager &displayManager, SceneManager &sceneManager );
        void shutDown();

        //Performs all high level prep operations that the graphics library
        //Needs to do before drawin each model in the scene.
        void render();

    private:
        void buildRenderQueue();
        bool initSoftwareRenderer();
        
        //This is a pointer to a pointer to allow for scene switching 
        SceneManager   * sceneLocator;
        DisplayManager * screen;

        SoftwareRenderer renderInstance;
        std::queue<Model*> *renderObjectQueue;        
};




#endif