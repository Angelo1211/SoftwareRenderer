#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <vector3.h>

struct TransformParameters{
    TransformParameters() :scaling(Vector3(1,1,1)) {};
    Vector3 translation;
    Vector3 rotation;
    Vector3 scaling;
};

class Matrix4{
    public:
        float& operator()(size_t y, size_t x){
            return mMatrix[y*4 + x];
        }
        Matrix4 operator* (Matrix4 &rhs);

        Vector3 matMultVec(Vector3 &vec);
        
        void printMat();

        //Probably should be their own classes or constructors in the future
        Matrix4 static makeTestMat();
        Matrix4 static makeFullRotMat(float alpha, float beta, float gamma);
        Matrix4 static makeScaleMat(float scaleX, float scaleY, float scaleZ);
        Matrix4 static makeTranslateMat(float dx, float dy, float dz);

        //Builds a matrix that rotates, scakes abd translates all in one
        Matrix4 static transformMatrix(TransformParameters transform);

        //Camera transformation matrix (the world from the camera's eyes)
        Matrix4 static lookAt(Vector3& position, Vector3& target, Vector3& temp);

        //2D to 3D projection matrix
        Matrix4 static makeProjectionMatrix(float fov, float AR, float near, float far);

        Matrix4(){};

    private:
        
        std::array<float, 16> mMatrix{};
};


#endif