#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "vector3D.h"

//Data struct holding all of the data you need to make a transform matrix
struct TransformParameters{
    TransformParameters() :scaling(Vector3f(1,1,1)) {};
    Vector3f translation;
    Vector3f rotation;
    Vector3f scaling;
};

//Matrices are stored in memory in row major order, but operations are done as if it was
//Column major. 
class Matrix4{
    public:
        //Operators
        float& operator()(size_t y, size_t x){
            return mMatrix[y*4 + x];
        }
        Matrix4 operator* (Matrix4 &rhs);
        Vector3f matMultVec(const Vector3f &vec); 
        Vector3f matMultDir(const Vector3f &vec);
        Matrix4 transpose();
        Matrix4 inverse();

        //Named constructor idiom to build the basic matrices we need for 
        //transformation.
        Matrix4 static makeTestMat();
        Matrix4 static unitMatrix(); 
        
        //Uses ZYX convention for rotation
        Matrix4 static fullRotMat(float alpha, float beta, float gamma);
        Matrix4 static scaleMat(float scaleX, float scaleY, float scaleZ);
        Matrix4 static translateMat(float dx, float dy, float dz);
        Matrix4(){};

        //Builds a matrix that scales, rotates and translates all at once
        Matrix4 static transformMatrix(TransformParameters transform);

        //Inverse Camera transformation matrix (the world from the camera's eyes)
        Matrix4 static lookAt(Vector3f& position, Vector3f& target, Vector3f& temp);

        //3D projection matrix. When applied results in the camera frustrum area being 
        //defined as X[-1,1] Y[-1,1] Z[1,0] 
        Matrix4 static projectionMatrix(float fov, float AR, float near, float far);

        Matrix4 static TBNMatrix(const Vector3f &tangent, const Vector3f &biTangent, const Vector3f &normal);

        //Debug stuff
        void print();
    private:
        std::array<float, 16> mMatrix{};
};


#endif