#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "vector3.h"


struct Camera{
    public:
        Camera();

        void update();

        //Position and direction of camera
        Vector3 position{0,0,8};
        Vector3 target;
        Vector3 up{0,1,0};

        //Matrices and frustrum stuff
        Matrix4 viewMatrix;
        Matrix4 projectionMatrix;
        int fov{75};
        int near{1};
        int far{100};
        float aspectRatio{1.0};
};

#endif