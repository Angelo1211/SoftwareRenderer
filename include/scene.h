#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "model.h"
#include "camera.h"

//Keeps track of all the lights, models and cameras contained in a current
//scene. Also performs frustrumc ulling to determine what objects to send to
//the render queue visibility.
class Scene{
    public:
        //Builds a scene based on a path to the folder containing the scene's
        //content
        Scene(std::string path);
        ~Scene();

        //Updates all models, lights and cameras in scene
        void update();

        //Returns the list of models not culled by the frustrum
        std::vector<Model*>* getVisiblemodels();

        Camera * getCurrentCamera();

        //Used in the scene loading check to determine if models were loaded 
        //correctly. It sends this info upstream to the scene manager to abort
        //the load procedure.
        bool checkIfEmpty();
    private:
        Camera mainCamera;

        bool emptyScene;

        std::vector<Model*> modelsInScene;

        //Contains the models that remain after frustrum culling
        std::vector<Model*> visibleModels;
        
        bool loadSceneModels(std::string &path);

        //Cull objects that should not be visible and add the visible to the 
        //visibleModels list for rendering TO DO 
        void frustrumCulling();
};

#endif