#include "camera.h"
#include "SDL.h"


Camera::Camera(){
    viewMatrix = Matrix4::lookAt(position,target,up);
    projectionMatrix = Matrix4::projectionMatrix(cameraFrustrum.fov, cameraFrustrum.AR, cameraFrustrum.near, cameraFrustrum.far);
    cameraFrustrum.setCamInternals();
    cameraFrustrum.updatePlanes(viewMatrix, position);
}

void Camera::update(){
    float t = static_cast<float>(SDL_GetTicks());
    float radius = 1.4;
    float camX   = std::sin(t/10000) * radius;
    float camZ   = std::cos(t/10000) * radius;
    position.x   = camX;
    position.y   = 0;
    position.z   = camZ;
    target.z     = 0;
    viewMatrix   = Matrix4::lookAt(position,target,up);
    cameraFrustrum.updatePlanes(viewMatrix, position);
}

bool Camera::checkVisibility(AABox *bounds){
    return cameraFrustrum.checkIfInside(bounds);
}