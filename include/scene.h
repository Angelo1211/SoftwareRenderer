#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "model.h"
#include "camera.h"
#include <vector>

class Scene{
    public:
        //Builds a scene based on a path containing the scene's content
        Scene(std::string path);
        ~Scene();

        //Updates all objects and cameras in scene
        void update();

        std::vector<Model*>* getVisiblemodels();

        Camera * getCurrentCamera();
    private:
        Camera mainCamera;

        std::vector<Model*> modelsInScene;

        //Contains the models that remain after frustrum culling
        std::vector<Model*> visibleModels;
        
        //Initializes all modelsin the scene
        void loadSceneModels(std::string path);

        //Cull objects that should not be visible and add the visible to the 
        //visibleModels list for rendering TO DO 
        void frustrumCulling();
};

#endif