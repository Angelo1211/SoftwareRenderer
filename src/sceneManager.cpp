#include "sceneManager.h"

//Dummy constructors / destructors
SceneManager::SceneManager(){}
SceneManager::~SceneManager(){}

bool SceneManager::startUp(){
    currentSceneID = "teapot";
    if (!loadScene(currentSceneID)){
        printf("Could not load scene. No models succesfully loaded!\n");
        return false;
    }
    return true;
}

void SceneManager::shutDown(){
    delete currentScene;
}

bool SceneManager::switchScene(std::string newSceneID){
    if( newSceneID != currentSceneID ){
        currentSceneID = newSceneID;
        delete currentScene;
        return loadScene(newSceneID);
    }
    else{
        printf("Selected already loaded scene.\n");
        return true;
    }
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