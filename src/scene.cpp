#include <scene.h>

Scene::Scene(std::string path){
    loadSceneModels(path);
}

Scene::~Scene(){
    for(Model *models : modelsInScene){
        delete models;
    }
}

void Scene::update(){
    visibleModels.clear();
    mainCamera.update();
    for(Model *models : modelsInScene){
        models->update();
    }
    frustrumCulling();
}

void Scene::loadSceneModels(std::string path){
    //In the future I want to read all o the models in the model folder
    //And build them here.  For now only one
    std::string fullPath = "../models/";
    fullPath = fullPath + path;
    modelsInScene.push_back(new Model(fullPath));

    //We also initialize the model position here position here
    TransformParameters initParameters;
    //initParameters.scaling = Vector3(1, 60, 60);
    initParameters.rotation = Vector3(0,0,0);
    initParameters.translation = Vector3(0, -1, 0);

    modelsInScene[0]->initPosition(initParameters);

    //sceneModel->describeMesh();
}

void Scene::frustrumCulling(){
    bool visible = true;
    for(Model *model : modelsInScene){

        //TO DO Visibility check against camera's frustrum planes

        if (visible){
            visibleModels.push_back(model);
        }
    }
}

std::vector<Model*>* Scene::getVisiblemodels(){
    return &visibleModels;
}

Camera* Scene::getCurrentCamera(){
    return &mainCamera;
}

// //If the frustrum culling returns true it means the model has been culled
    // //Because no other models are in the scene we return
    // // if (frustrumCulling(models, viewMatrix)){
    // //     printf("Model culled!\n");
    // //     return;
    // // } 


//     //stupid frustrum culling
    //     //if(v1.x < -1 || v1.x > 1 || v1.y < -1 || v1.y > 1 || v1.z > 1 || v1.z < -1) continue;
    //     //if(v2.x < -1 || v2.x > 1 || v2.y < -1 || v2.y > 1 || v2.z > 1 || v2.z < -1) continue;
    //     //if(v3.x < -1 || v3.x > 1 || v3.y < -1 || v3.y > 1 || v3.z > 1 || v3.z < -1) continue;