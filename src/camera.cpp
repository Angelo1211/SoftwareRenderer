#include "camera.h"

Camera::Camera(){
    side = front.crossProduct(up).normalized();
    viewMatrix = Matrix4::lookAt(position, target, up);
    projectionMatrix = Matrix4::projectionMatrix(cameraFrustrum.fov, cameraFrustrum.AR, cameraFrustrum.near, cameraFrustrum.far);
    cameraFrustrum.setCamInternals();
    cameraFrustrum.updatePlanes(viewMatrix, position);
}

void Camera::update(unsigned int deltaT){
    if(orbiting){
        float t = static_cast<float>(SDL_GetTicks());
        float camX   = std::sin(t/6000) * radius;
        float camZ   = std::cos(t/6000) * radius;
        position.x   = camX;
        position.y   = camX;
        position.z   = camZ;
    }
    else{
        target = position + front;
    }
    viewMatrix   = Matrix4::lookAt(position,target,up);
    cameraFrustrum.updatePlanes(viewMatrix, position);
}

bool Camera::checkVisibility(AABox *bounds){
    return cameraFrustrum.checkIfInside(bounds);
}