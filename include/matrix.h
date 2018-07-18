#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <vector3.h>

//Data struct holding all of the data you need to make a transform matrix
struct TransformParameters{
    TransformParameters() :scaling(Vector3(1,1,1)) {};
    Vector3 translation;
    Vector3 rotation;
    Vector3 scaling;
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
        Vector3 matMultVec(Vector3 &vec); 
        
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
        Matrix4 static lookAt(Vector3& position, Vector3& target, Vector3& temp);

        //3D projection matrix. When applied results in the camera frustrum area being 
        //defined as X[-1,1] Y[-1,1] Z[1,0] 
        Matrix4 static projectionMatrix(float fov, float AR, float near, float far);

        //Debug stuff
        void print();
    private:
        std::array<float, 16> mMatrix{};
};


#endif