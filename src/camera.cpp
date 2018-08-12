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
        float ang    = 2 * M_PI * static_cast<float>(SDL_GetTicks()) / 3e4;
        float camX   = std::sin(ang) * radius; 
        float camZ   = std::cos(ang) * radius;
        position.x   = camX;
        position.y   = camX;
        position.z   = camZ;
    }
    else{
        target = position + front;
    }
    viewMatrix = Matrix4::lookAt(position, target, up);
    cameraFrustrum.updatePlanes(viewMatrix, position);
    projectionMatrix = Matrix4::projectionMatrix(cameraFrustrum.fov, cameraFrustrum.AR, cameraFrustrum.near, cameraFrustrum.far);
}

bool Camera::checkVisibility(AABox *bounds){
    return cameraFrustrum.checkIfInside(bounds);
}

void Camera::resetCamera(){
    position = Vector3f(0, 0, 8.0);
    target.zero();  
    front    = Vector3f(0, 0, -1);
    side     = front.crossProduct(up);
    radius   =   2;
    pitch    =   0;
    yaw      = -90;
}