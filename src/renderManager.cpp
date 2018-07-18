#include "renderManager.h"
#include <vector>
#include "model.h"

//Dummy constructors / Destructors
RenderManager::RenderManager(){}
RenderManager::~RenderManager(){}

bool RenderManager::startUp(DisplayManager &displayManager,SceneManager &sceneManager ){
    screen = &displayManager;
    sceneLocator = &sceneManager;
    if( !initSoftwareRenderer() ){
        printf("Failed to initialize software Renderer!\n");
        return false;
    }
    return true;
}

void RenderManager::shutDown(){
    sceneLocator = nullptr;
    screen = nullptr;
    renderInstance.shutDown();
}

void RenderManager::render(){
    //Clear screen and reset current buffers
    screen->clear();
    renderInstance.clearBuffers();

    //Build a render Queue for drawing multiple models
    //Also assigns the current camera to the software renderer
    buildRenderQueue();

    //Draw all meshes in the render queue so far we assume they are
    //normal triangular meshes.
    while( !renderQueue.empty() ){
        renderInstance.drawTriangularMesh(renderQueue.front());
        renderQueue.pop();
    }

    //Swapping pixel buffer with final rendered image with the
    //display buffer
    screen->swapBuffers(renderInstance.getRenderTarget());

    //Set camera pointer to null just in case a scene change occurs
    renderInstance.setCameraToRenderFrom(nullptr);
}

//Gets the list of visible models from the current scene and 
//extracts a list of pointers to their meshes. 
//Done every frame in case scene changes
void RenderManager::buildRenderQueue(){
    
    //Set renderer camera
    Scene* currentScene = sceneLocator->getCurrentScene();
    renderInstance.setCameraToRenderFrom(currentScene->getCurrentCamera());

    //Get pointers to the visible models
    std::vector<Model*>* visibleModels = currentScene->getVisiblemodels();
    
    //Builds render queue from visibleModels list
    for (Model* model : *visibleModels ){
        renderQueue.push(model->getMesh());
    }
}

bool RenderManager::initSoftwareRenderer(){
    int w = DisplayManager::SCREEN_WIDTH;
    int h = DisplayManager::SCREEN_HEIGHT;
    return renderInstance.startUp(w,h);
}








