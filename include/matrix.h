#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <array>
#include <vector3.h>

class Matrix{
    public:
        float& operator()(size_t y, size_t x){
            return mMatrix[y*3 + x];
        }

        void makeRotX(float theta);

        Vector3 matXVec(Vector3 &vertex);

    private:
        std::array<float, 9>  mMatrix{0,0,0,0,0,0,0,0,0};
};


// template<class T>
// class Mat4 : public MatrixM<T,4,4>{
//     public:
//         void fullRotMat(T thetaX, T thetaY, T thetaZ);
        
// };



#endif