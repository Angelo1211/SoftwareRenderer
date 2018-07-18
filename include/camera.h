#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "vector3.h"


struct Camera{
    Camera();

    //In the future user input should control this. For now just simple movement
    void update();

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    //Position and direction of camera, used to build view matrix
    Vector3 position{0,0,8};
    Vector3 target{0,0,0};
    Vector3 up{0,1,0};

    //Variables that determine frustrum (Future class?)
    //Used to build projection matrix
    float fov{90};
    float near{0.1};
    float far{100};
    float aspectRatio{1.0};
};

#endif