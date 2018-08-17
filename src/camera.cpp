// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// ===============================

//Headers
#include "camera.h"

Camera::Camera(){
    side = front.crossProduct(up).normalized();
    viewMatrix = Matrix4::lookAt(position, target, up);
    projectionMatrix = Matrix4::projectionMatrix(cameraFrustrum.fov, cameraFrustrum.AR, cameraFrustrum.near, cameraFrustrum.far);
    cameraFrustrum.setCamInternals();
    cameraFrustrum.updatePlanes(viewMatrix, position);
}

//Updates target and position based on camera movement mode.
///Also updates view matrix and projection matrix for rendering
void Camera::update(unsigned int deltaT){
    if(orbiting){
        float ang    = 2 * M_PI * static_cast<float>(SDL_GetTicks()) / (period*1000);
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

//View frustrum culling using a models AAB
bool Camera::checkVisibility(AABox *bounds){
    return cameraFrustrum.checkIfInside(bounds);
}

//Used by input to reset camera to origin in case user loses their bearings
void Camera::resetCamera(){
    position = Vector3f(0, 0, 8.0);
    target.zero();  
    front    = Vector3f(0, 0, -1);
    side     = front.crossProduct(up);
    radius   =   2;
    pitch    =   0;
    yaw      = -90;
    period   =  30;
}