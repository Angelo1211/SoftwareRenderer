#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "scene.h"
//Add an enum in the future with the different scenes that it should be able to
//render
class SceneManager{

    public:
        //Dummy Constructor / Destructor
        SceneManager();
        ~SceneManager();

        //Initializes and closes all scene related stuff
        bool startUp();
        void shutDown();

        // Scene switching
        bool switchScene();

        // Update current scene
        void update();

        //Called by the rendermanager to prep the render queue 
        Scene* getCurrentScene();
    private:
        bool loadScene();
        bool closeScene();

        Scene *currentScene;
};


#endif