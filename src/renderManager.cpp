#include "renderManager.h"


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
    while( !renderObjectQueue->empty() ){
        renderInstance.drawTriangularMesh(renderObjectQueue->front());
        renderObjectQueue->pop();
    }

    //Swapping pixel buffer with final rendered image with the
    //display buffer
    screen->swapBuffers(renderInstance.getRenderTarget());

    //Set camera pointer to null just in case a scene change occurs
    renderInstance.setCameraToRenderFrom(nullptr);
}

//Gets the list of visible models from the current scene
//Done every frame in case scene changes
void RenderManager::buildRenderQueue(){

    //set scene info
    Scene* currentScene = sceneLocator->getCurrentScene();
    
    //Set renderer camera
    renderInstance.setCameraToRenderFrom(currentScene->getCurrentCamera());

    //Update the pointer to the list of lights in the scene
    renderInstance.setSceneLights(currentScene->getCurrentLights(), currentScene->getLightCount() );

    //Get pointers to the visible model queu
    renderObjectQueue = currentScene->getVisiblemodels();
}

bool RenderManager::initSoftwareRenderer(){
    int w = DisplayManager::SCREEN_WIDTH;
    int h = DisplayManager::SCREEN_HEIGHT;
    return renderInstance.startUp(w,h);
}










