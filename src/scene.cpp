#include "scene.h"
#include "objParser.h"
#include <sys/stat.h>
#include <sys/types.h>

struct stat info;

//For now a scene only contains a single model
Scene::Scene(const std::string &sceneFolder){

    emptyScene = loadSceneModels(sceneFolder);
}

Scene::~Scene(){
    //Making sure you don't attempt to delete models that don't exist
    if (!emptyScene){
        for(Model *models : modelsInScene){
            delete models;
        }
    }
}

void Scene::update(){
    mainCamera.update();
    for(Model *models : modelsInScene){
        models->update();
    }
    frustrumCulling();
}

//Check if the scene exists and if it contains any valid mesh
bool Scene::loadSceneModels(const std::string &sceneFolder){
    std::string path = "../scenes/";
    path +=  sceneFolder;

    //Check if the current scene folder acually exists both in windows and linux
    //And also checks if it can be accessed
    if( stat( path.c_str(), &info ) != 0 ){
        printf( "cannot access %s\n", path.c_str() );
    }
    else if( info.st_mode & S_IFDIR ){
        printf( "%s is a valid scene\n", path.c_str() );

        //Build a base file path that all models will use 
        //Use it to do an early quit in case the mesh does not exist
        std::string baseFilePath = path + "/" + sceneFolder;
        std::string meshFilePath = baseFilePath + "_mesh.obj";
        if(!OBJ::fileExists(meshFilePath)){
            printf("Error! Mesh: %s does not exist.\n",meshFilePath.c_str());
            return true;
        }
        else{
            printf( "%s is a valid mesh\n", meshFilePath.c_str() );
            TransformParameters initParameters;
            initParameters.rotation = Vector3f(90*M_PI/180.0f, 0 , 0);
            // initParameters.translation = Vector3f(0, -0.5, 0);
            initParameters.scaling = Vector3f(0.3, 0.3, 0.3);
            initParameters.translation = Vector3f(0, 0, 0);
            modelsInScene.push_back(new Model(baseFilePath, initParameters));
            return false;
        }

    }
    else{
        printf("Error! Scene: %s does not exist.\n",path.c_str());
        return true;
    }
}

void Scene::frustrumCulling(){
    bool visible = true;
    for(Model *model : modelsInScene){

        visible = mainCamera.checkVisibility(model->getBounds());

        if (visible) {
            visibleModels.push(model);
        }
    }
}

 std::queue<Model*>* Scene::getVisiblemodels(){
    return &visibleModels;
}

Camera* Scene::getCurrentCamera(){
    return &mainCamera;
}

bool Scene::checkIfEmpty(){
    return emptyScene;
}