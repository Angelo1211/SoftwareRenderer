#include "camera.h"
#include "SDL.h"
#include "displayManager.h"

Camera::Camera(){
    viewMatrix = Matrix4::lookAt(position,target,up);
    aspectRatio =  DisplayManager::SCREEN_ASPECT_RATIO;
    projectionMatrix = Matrix4::projectionMatrix(fov, aspectRatio, near,far);
}

void Camera::update(){
    float t = static_cast<float>(SDL_GetTicks());
    float radius = 5;
    float camX   = std::sin(t/4000) * radius;
    float camZ   = std::cos(t/4000) * radius;
    position.x   = 0;
    position.y   = 0;
    position.z   = camZ;
    viewMatrix   = Matrix4::lookAt(position,target,up);
}