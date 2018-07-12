#include "camera.h"
#include "SDL.h"
#include "displayManager.h"

Camera::Camera(){
    viewMatrix = Matrix4::lookAt(position,target,up);
    aspectRatio =  DisplayManager::SCREEN_WIDTH/(float)DisplayManager::SCREEN_HEIGHT;
    projectionMatrix = Matrix4::makeProjectionMatrix(fov, aspectRatio, near,far);
}

void Camera::update(){
    float t = static_cast<float>(SDL_GetTicks());
    float radius = 8;
    float camX   = std::sin(t/4000) * radius;
    float camZ   = std::cos(t/4000) * radius;
    position.x = camX;
    position.y = 0;
    position.z = camZ;
    viewMatrix = Matrix4::lookAt(position,target,up);
    viewMatrix = (Matrix4::makeTranslateMat(0,camX*0.25,0)*viewMatrix);
}