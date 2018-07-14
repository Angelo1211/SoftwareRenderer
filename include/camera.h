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
        Vector3 target{0,0,0};
        Vector3 up{0,1,0};

        //Matrices and frustrum stuff
        Matrix4 viewMatrix;
        Matrix4 projectionMatrix;
        float fov{90};
        //Given in positive values but changed to negative in the matrix
        float near{0.1};
        float far{100};
        float aspectRatio{1.0};
};

#endif