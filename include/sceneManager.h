#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// PURPOSE      : Managing the switching of scenes, loading unloading etc, and updating
//                the current scene. It also returns the currently loaded scene to any
//                other manager system that wants that information. 
// ===============================
// SPECIAL NOTES: The reason I believe you need a scene manager class and not have the 
// the scenes do all of this themselves is that I don't want to have to update all 
// pointers to the scene every time that changes. Instead you just point to the scenemanager
// and that keeps track of the scene itself.
// ===============================

//Headers
#include "scene.h"

class SceneManager{
    public:
        //Dummy Constructor / Destructor
        SceneManager();
        ~SceneManager();

        //Initializes and closes all scene related stuff
        bool startUp();
        void shutDown();

        // Scene switching
        bool switchScene(std::string sceneID);

        // Update current scene
        void update(unsigned int deltaT);

        //Called by the rendermanager to prep the render queue 
        //Also called by the input manager as a precaution to avoid dangling pointers
        Scene* getCurrentScene();

    private:
        bool loadScene(std::string sceneID);

        //String could probably be an enum instead, but it's easier this way to build
        //the relative paths if it is a string.
        std::string currentSceneID;
        Scene* currentScene;
};

#endif