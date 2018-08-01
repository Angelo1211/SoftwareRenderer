#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include <queue>
#include "model.h"
#include "camera.h"


//Keeps track of all the lights, models and cameras contained in a current
//scene. Also performs frustrumc ulling to determine what objects to send to
//the render queue visibility.
class Scene{
    public:
        //Builds a scene based on a path to the folder containing the scene's
        //content
        Scene(const std::string &sceneFolder);
        ~Scene();

        //Updates all models, lights and cameras in scene
        void update();

        //Returns the list of models not culled by the frustrum
        std::queue<Model*>* getVisiblemodels();

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
        std::queue<Model*> visibleModels;
        
        bool loadSceneModels(const std::string &sceneFolder);

        //Cull objects that should not be visible and add the visible to the 
        //visibleModels list for rendering TO DO 
        void frustrumCulling();
};

#endif