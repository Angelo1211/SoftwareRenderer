#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "vector3D.h"
#include "geometry.h"
#include "displayManager.h"

struct Camera{
    Camera();
    
    bool checkVisibility(AABox *bounds);

    //Updates the camera matrices with the user input obtained in the input class
    void update(unsigned int deltaT);
    void resetCamera();

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    //Position and direction of camera, used to build view matrix
    Vector3f position{0,0,8};
    Vector3f target{0,0,0};
    Vector3f up{0,1,0};
    Vector3f front{0, 0, -1};
    Vector3f side;

    //Values related to orbiting mode
    float radius  = 2;
    bool orbiting = true;

    //Momentary fixed camera speed (FPS dependent)
    float camSpeed = 0.1f;
    float pitch    = 0;
    float yaw      = -90;

    Frustrum cameraFrustrum{DisplayManager::SCREEN_ASPECT_RATIO};
};

#endif