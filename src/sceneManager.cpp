#include "sceneManager.h"

SceneManager::SceneManager(){
    
}

SceneManager::~SceneManager(){
    
}

bool SceneManager::startUp(){
    if (!loadScene()){
        printf("Could not load scene.\n");
        return false;
    }
    return true;
}

void SceneManager::shutDown(){
    delete currentScene;
}

//Not implemented yet, but I will want to do this in the future
bool SceneManager::switchScene(){
    return true;
}

//for now just loads the teapot.obj
bool SceneManager::loadScene(){
    currentScene = new Scene("teapot.obj");
    return  !currentScene->checkIfEmpty();
}

void SceneManager::update(){
    currentScene->update();
}

Scene* SceneManager::getCurrentScene(){
    return currentScene;
}