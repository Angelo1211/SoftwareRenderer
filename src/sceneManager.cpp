#include "sceneManager.h"

//Dummy constructors / destructors
SceneManager::SceneManager(){}
SceneManager::~SceneManager(){}

bool SceneManager::startUp(){
    if (!loadScene()){
        printf("Could not load scene. No models succesfully loaded!\n");
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

//for now just loads a single .obj based on the  given string
bool SceneManager::loadScene(){
    currentScene = new Scene("cow.obj");
    return  !currentScene->checkIfEmpty(); //True if empty, so it's negated for startup
}

void SceneManager::update(){
    currentScene->update();
}

Scene* SceneManager::getCurrentScene(){
    return currentScene;
}