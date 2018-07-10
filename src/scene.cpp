#include <scene.h>

Scene::Scene(std::string path){
    loadSceneModels(path);
}

Scene::~Scene(){
    delete sceneModel;
}

void Scene::update(){
    mainCamera.update();
    sceneModel->update();
}

void Scene::loadSceneModels(std::string path){
    //In the future I want to read all o the models in the model folder
    //And build them here.  For now I force it to be only one.
    std::string fullPath = "../models/";
    fullPath = fullPath + path;
    sceneModel = new Model(fullPath);

    //We also initialize the model position here position here
    TransformParameters initParameters;
    //initParameters.scaling = Vector3(1, 60, 60);
    initParameters.rotation = Vector3(0,0,0);
    initParameters.translation = Vector3(0, -1, 0);

    sceneModel->initPosition(initParameters);

    //sceneModel->describeMesh();
}