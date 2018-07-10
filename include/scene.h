#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "model.h"
#include "camera.h"

class Scene{
    public:
        //Builds a scene based on a path containing the scene's content
        Scene(std::string path);
        ~Scene();

        //Updates all objects and cameras in scene
        void update();
    private:
        Camera mainCamera;
        Model *sceneModel;
        void loadSceneModels(std::string path);
};

#endif