#include "sceneManager.h"

//Dummy constructors / destructors
SceneManager::SceneManager(){}
SceneManager::~SceneManager(){}

bool SceneManager::startUp(){
    if (!loadScene("teapot")){
        printf("Could not load scene. No models succesfully loaded!\n");
        return false;
    }
    return true;
}

void SceneManager::shutDown(){
    delete currentScene;
}

//Not implemented yet, but I will want to do this in the future
bool SceneManager::switchScene(std::string sceneID){
    delete currentScene;
    return loadScene(sceneID);
}

//Loads the given scene and all related textures
bool SceneManager::loadScene(std::string sceneID){
    currentScene = new Scene(sceneID);
    return  !currentScene->checkIfEmpty(); //True if empty, so it's negated for startup
}

void SceneManager::update(){
    currentScene->update();
}

Scene* SceneManager::getCurrentScene(){
    return currentScene;
}