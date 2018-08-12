#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "vector3D.h"
#include "geometry.h"
#include "displayManager.h"

struct Camera{
    Camera();
    
    bool checkVisibility(AABox *bounds);

    //In the future user input should control this. For now just simple movement
    void update(unsigned int deltaT);

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    //Position and direction of camera, used to build view matrix
    Vector3f position{0,0,8};
    Vector3f target{0,0,0};
    Vector3f up{0,1,0};
    Vector3f front{0, 0, -1};
    Vector3f side;

    //Values related to orbiting mode
    float radius = 2;
    bool orbiting = false;

    //Momentary fixed camera speed (FPS dependent)
    float camSpeed = 0.1f;

    Frustrum cameraFrustrum{DisplayManager::SCREEN_ASPECT_RATIO};
};

#endif