#include "scene.h"
#include "objParser.h"

Scene::Scene(std::string path){
    emptyScene = loadSceneModels(path);
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

bool Scene::loadSceneModels(std::string &path){
    //In the future I want to read all o the models in the model folder
    //And build them here. For now only one is loaded.
    std::string fullPath = "../models/";
    fullPath = fullPath + path;

    if(!OBJ::fileExists(fullPath)){
        printf("Error! File:%s does not exist.\n",path.c_str());
        return true;
    }
    else{
        TransformParameters initParameters;
        //initParameters.rotation = Vector3f(90*M_PI/180.0f, 0 , 0);
        initParameters.translation = Vector3f(0, -0.5, 0);
        modelsInScene.push_back(new Model(fullPath, initParameters));
        return false;
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