#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <vector3.h>

struct transformParameters{
    Vector3 position;
    Vector3 rotation;
    Vector3 scaling;
};

class Matrix4{
    public:
        float& operator()(size_t y, size_t x){
            return mMatrix[y*4 + x];
        }
        Matrix4 operator* (Matrix4 rhs);

        Vector3 matMultVec(Vector3 &vec, float w);
        
        void printMat();

        //Probably should be their own classes or constructors in the future
        Matrix4 static makeTestMat();
        Matrix4 static makeFullRotMat(float alpha, float beta, float gamma);
        Matrix4 static makeScaleMat(float scaleX, float scaleY, float scaleZ);
        Matrix4 static makeTranslateMat(float dx, float dy, float dz);
        Matrix4 static modelMatrix(Vector3 position, Vector3 rotation, Vector3 scaling);

    private:
        Matrix4(){};
        std::array<float, 16> mMatrix{};
};


#endif