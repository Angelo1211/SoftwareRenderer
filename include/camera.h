#ifndef CAMERA_H
#define CAMERA_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-10
// PURPOSE      : To contain all camera and visibility related data. Also perform 
//                view frustrum culling of a given AABB against the six planes of the
//                camera frustrum. It allows for two types of movement, user controlled
//                or orbiting mode. 
// ===============================
// SPECIAL NOTES: Some of the vectors saved in the camera are somewhat redundant, but
// they are kept because I believe that recalculating them each frame is not really
// worth it performance wise. I haven't profiled it however so who knows.
// ===============================

//Headers
#include "matrix.h"
#include "vector3D.h"
#include "geometry.h"
#include "displayManager.h"

struct Camera{
    Camera();

    //Visibility and geometry member variables
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    Frustrum cameraFrustrum{DisplayManager::SCREEN_ASPECT_RATIO};

    //View frustrum culling
    bool checkVisibility(AABox *bounds);

    //Updates the camera matrices with the user input obtained in the input class
    void update(unsigned int deltaT);
    void resetCamera(); 

    //Position and direction of camera, used to build view matrix
    Vector3f position{0,0,8};
    Vector3f target{0,0,0};
    Vector3f up{0,1,0};
    Vector3f front{0, 0, -1};
    Vector3f side;

    //Values related to orbiting mode
    float radius  = 2;
    float period  = 30; //in seconds
    bool orbiting = true;

    //Momentary fixed camera speed (FPS dependent)
    float camSpeed = 0.1f;
    float pitch    = 0;
    float yaw      = -90;
};

#endif