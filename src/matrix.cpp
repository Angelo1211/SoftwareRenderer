// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-04
// ===============================

//Headers
#include "matrix.h"
#include <math.h>

//The full matrix vector multiplication routine 
//using the full 4x4 values.
//Also changes the secret w component in the vec3 to w 
//which is only really ever used in perspective divide and clipping
Vector3f Matrix4::matMultVec(const Vector3f &vec){
    Vector3f newVec(0,0,0);
    float w2 = 0;
    newVec.x = vec.x*(*this)(0,0)+
               vec.y*(*this)(0,1)+
               vec.z*(*this)(0,2)+
               (*this)(0,3); //Assuming wO of vec always  = 1

    newVec.y = vec.x*(*this)(1,0)+
               vec.y*(*this)(1,1)+
               vec.z*(*this)(1,2)+
               (*this)(1,3); //Assuming wO of vec always  = 1

    newVec.z = vec.x*(*this)(2,0)+
               vec.y*(*this)(2,1)+
               vec.z*(*this)(2,2)+
               (*this)(2,3); //Assuming wO of vec always  = 1

    w2       = vec.x*(*this)(3,0)+
               vec.y*(*this)(3,1)+
               vec.z*(*this)(3,2)+
               (*this)(3,3); //Assuming wO of vec always  = 1

    newVec.w = w2;

    return newVec;
}

//Used to multiply direction vectors or when you do not what your vector to be
//translated. Implicitely makes the matrix a 3x3
Vector3f Matrix4::matMultDir(const Vector3f &vec){
    Vector3f newVec(0,0,0);
    newVec.x = vec.x*(*this)(0,0)+
               vec.y*(*this)(0,1)+
               vec.z*(*this)(0,2); //Assuming wO of vec always  = 1

    newVec.y = vec.x*(*this)(1,0)+
               vec.y*(*this)(1,1)+
               vec.z*(*this)(1,2); //Assuming wO of vec always  = 1

    newVec.z = vec.x*(*this)(2,0)+
               vec.y*(*this)(2,1)+
               vec.z*(*this)(2,2);; //Assuming wO of vec always  = 1

    return newVec;
}

Matrix4 Matrix4::transpose(){
    Matrix4 transposeMatrix;
    //First row
    transposeMatrix(0,0) = (*this)(0,0);
    transposeMatrix(0,1) = (*this)(1,0);
    transposeMatrix(0,2) = (*this)(2,0);
    transposeMatrix(0,3) = (*this)(3,0);

    //Second row
    transposeMatrix(1,0) = (*this)(0,1);
    transposeMatrix(1,1) = (*this)(1,1);
    transposeMatrix(1,2) = (*this)(2,1);
    transposeMatrix(1,3) = (*this)(3,1);

    //Third row
    transposeMatrix(2,0) = (*this)(0,2);
    transposeMatrix(2,1) = (*this)(1,2);
    transposeMatrix(2,2) = (*this)(2,2);
    transposeMatrix(2,3) = (*this)(3,2);

    //Fourth row
    transposeMatrix(3,0) = (*this)(0,3);
    transposeMatrix(3,1) = (*this)(1,3);
    transposeMatrix(3,2) = (*this)(2,3);
    transposeMatrix(3,3) = (*this)(3,3);
    return transposeMatrix;
}

//Taken from Mesa implementation of GLU library
Matrix4 Matrix4::inverse(){
    Matrix4 inverseMat;

    float det;
    int i, j;
    
    inverseMat(0,0) = mMatrix[5]  * mMatrix[10] * mMatrix[15] - 
             mMatrix[5]  * mMatrix[11] * mMatrix[14] - 
             mMatrix[9]  * mMatrix[6]  * mMatrix[15] + 
             mMatrix[9]  * mMatrix[7]  * mMatrix[14] +
             mMatrix[13] * mMatrix[6]  * mMatrix[11] - 
             mMatrix[13] * mMatrix[7]  * mMatrix[10];

    inverseMat(1,0) = -mMatrix[4]  * mMatrix[10] * mMatrix[15] + 
              mMatrix[4]  * mMatrix[11] * mMatrix[14] + 
              mMatrix[8]  * mMatrix[6]  * mMatrix[15] - 
              mMatrix[8]  * mMatrix[7]  * mMatrix[14] - 
              mMatrix[12] * mMatrix[6]  * mMatrix[11] + 
              mMatrix[12] * mMatrix[7]  * mMatrix[10];

    inverseMat(2,0) = mMatrix[4]  * mMatrix[9] * mMatrix[15] - 
             mMatrix[4]  * mMatrix[11] * mMatrix[13] - 
             mMatrix[8]  * mMatrix[5] * mMatrix[15] + 
             mMatrix[8]  * mMatrix[7] * mMatrix[13] + 
             mMatrix[12] * mMatrix[5] * mMatrix[11] - 
             mMatrix[12] * mMatrix[7] * mMatrix[9];

    inverseMat(3,0) = -mMatrix[4]  * mMatrix[9] * mMatrix[14] + 
               mMatrix[4]  * mMatrix[10] * mMatrix[13] +
               mMatrix[8]  * mMatrix[5] * mMatrix[14] - 
               mMatrix[8]  * mMatrix[6] * mMatrix[13] - 
               mMatrix[12] * mMatrix[5] * mMatrix[10] + 
               mMatrix[12] * mMatrix[6] * mMatrix[9];

    inverseMat(0,1) = -mMatrix[1]  * mMatrix[10] * mMatrix[15] + 
              mMatrix[1]  * mMatrix[11] * mMatrix[14] + 
              mMatrix[9]  * mMatrix[2] * mMatrix[15] - 
              mMatrix[9]  * mMatrix[3] * mMatrix[14] - 
              mMatrix[13] * mMatrix[2] * mMatrix[11] + 
              mMatrix[13] * mMatrix[3] * mMatrix[10];

    inverseMat(1,1) = mMatrix[0]  * mMatrix[10] * mMatrix[15] - 
             mMatrix[0]  * mMatrix[11] * mMatrix[14] - 
             mMatrix[8]  * mMatrix[2] * mMatrix[15] + 
             mMatrix[8]  * mMatrix[3] * mMatrix[14] + 
             mMatrix[12] * mMatrix[2] * mMatrix[11] - 
             mMatrix[12] * mMatrix[3] * mMatrix[10];

    inverseMat(2,1) = -mMatrix[0]  * mMatrix[9] * mMatrix[15] + 
              mMatrix[0]  * mMatrix[11] * mMatrix[13] + 
              mMatrix[8]  * mMatrix[1] * mMatrix[15] - 
              mMatrix[8]  * mMatrix[3] * mMatrix[13] - 
              mMatrix[12] * mMatrix[1] * mMatrix[11] + 
              mMatrix[12] * mMatrix[3] * mMatrix[9];

    inverseMat(3,1)= mMatrix[0]  * mMatrix[9] * mMatrix[14] - 
              mMatrix[0]  * mMatrix[10] * mMatrix[13] - 
              mMatrix[8]  * mMatrix[1] * mMatrix[14] + 
              mMatrix[8]  * mMatrix[2] * mMatrix[13] + 
              mMatrix[12] * mMatrix[1] * mMatrix[10] - 
              mMatrix[12] * mMatrix[2] * mMatrix[9];

    inverseMat(0,2) = mMatrix[1]  * mMatrix[6] * mMatrix[15] - 
             mMatrix[1]  * mMatrix[7] * mMatrix[14] - 
             mMatrix[5]  * mMatrix[2] * mMatrix[15] + 
             mMatrix[5]  * mMatrix[3] * mMatrix[14] + 
             mMatrix[13] * mMatrix[2] * mMatrix[7] - 
             mMatrix[13] * mMatrix[3] * mMatrix[6];

    inverseMat(1,2) = -mMatrix[0]  * mMatrix[6] * mMatrix[15] + 
              mMatrix[0]  * mMatrix[7] * mMatrix[14] + 
              mMatrix[4]  * mMatrix[2] * mMatrix[15] - 
              mMatrix[4]  * mMatrix[3] * mMatrix[14] - 
              mMatrix[12] * mMatrix[2] * mMatrix[7] + 
              mMatrix[12] * mMatrix[3] * mMatrix[6];

    inverseMat(2,2) = mMatrix[0]  * mMatrix[5] * mMatrix[15] - 
              mMatrix[0]  * mMatrix[7] * mMatrix[13] - 
              mMatrix[4]  * mMatrix[1] * mMatrix[15] + 
              mMatrix[4]  * mMatrix[3] * mMatrix[13] + 
              mMatrix[12] * mMatrix[1] * mMatrix[7] - 
              mMatrix[12] * mMatrix[3] * mMatrix[5];

    inverseMat(3,2) = -mMatrix[0]  * mMatrix[5] * mMatrix[14] + 
               mMatrix[0]  * mMatrix[6] * mMatrix[13] + 
               mMatrix[4]  * mMatrix[1] * mMatrix[14] - 
               mMatrix[4]  * mMatrix[2] * mMatrix[13] - 
               mMatrix[12] * mMatrix[1] * mMatrix[6] + 
               mMatrix[12] * mMatrix[2] * mMatrix[5];

    inverseMat(0,3) = -mMatrix[1] * mMatrix[6] * mMatrix[11] + 
              mMatrix[1] * mMatrix[7] * mMatrix[10] + 
              mMatrix[5] * mMatrix[2] * mMatrix[11] - 
              mMatrix[5] * mMatrix[3] * mMatrix[10] - 
              mMatrix[9] * mMatrix[2] * mMatrix[7] + 
              mMatrix[9] * mMatrix[3] * mMatrix[6];

    inverseMat(1,3) = mMatrix[0] * mMatrix[6] * mMatrix[11] - 
             mMatrix[0] * mMatrix[7] * mMatrix[10] - 
             mMatrix[4] * mMatrix[2] * mMatrix[11] + 
             mMatrix[4] * mMatrix[3] * mMatrix[10] + 
             mMatrix[8] * mMatrix[2] * mMatrix[7] - 
             mMatrix[8] * mMatrix[3] * mMatrix[6];

    inverseMat(2,3) = -mMatrix[0] * mMatrix[5] * mMatrix[11] + 
               mMatrix[0] * mMatrix[7] * mMatrix[9] + 
               mMatrix[4] * mMatrix[1] * mMatrix[11] - 
               mMatrix[4] * mMatrix[3] * mMatrix[9] - 
               mMatrix[8] * mMatrix[1] * mMatrix[7] + 
               mMatrix[8] * mMatrix[3] * mMatrix[5];

    inverseMat(3,3) = mMatrix[0] * mMatrix[5] * mMatrix[10] - 
              mMatrix[0] * mMatrix[6] * mMatrix[9] - 
              mMatrix[4] * mMatrix[1] * mMatrix[10] + 
              mMatrix[4] * mMatrix[2] * mMatrix[9] + 
              mMatrix[8] * mMatrix[1] * mMatrix[6] - 
              mMatrix[8] * mMatrix[2] * mMatrix[5];

    det = mMatrix[0] * inverseMat(0,0) + mMatrix[1] * inverseMat(1,0) + mMatrix[2] * inverseMat(2,0) + mMatrix[3] * inverseMat(3,0);

    det = 1.0 / det;

    for (i = 0; i < 4; ++i){
        for (j = 0; j < 4; ++j){
            inverseMat(i,j) = inverseMat(i,j) * det;
        }   
    }

    return inverseMat;
}

//Way too general, should probably change in the future since I think
//I'll only need 4x4 stuff and this technically allows for any n matrix.
//Traverse the matrix cell by cell and find the final value through a step of 
//sub multiplications that are then added together
Matrix4 Matrix4::operator*(Matrix4 &rhs){
    //Matrix dimensions
    Matrix4 results;
    int n = 4;
    for(int rows = 0; rows < n; ++rows){
        for(int cols = 0; cols < n; ++cols){
            float total = 0;
            //total value of multiplication with all submultiplications added together
            //sub represents submultiplications in the actual matrix multiplication
            //For a nxn matrix you have n submultiplications
            for(int sub = 1; sub < n+1; ++sub ){
                int rowLhs = rows; //row ind left matrix
                int colLhs = sub - 1; //col ind left matrix
                int rowRhs = sub - 1; //row ind right matrix
                int colRhs = cols; //col ind right matrix

                total += (*this)(rowLhs,colLhs) * rhs(rowRhs,colRhs);
            }
            results(rows,cols) = total;
        }
    }
    return results;
}

// 1-15 values 
Matrix4 Matrix4::makeTestMat(){
    Matrix4 testMat;
    int n = 4;
    int val = 1;
    for(int rows = 0; rows < n; ++rows){
        for(int cols = 0; cols < n; ++cols){
            testMat(rows,cols) = val;
            ++val;
        }
    }
    return testMat;
}

Matrix4 Matrix4::unitMatrix(){
    Matrix4 testMat;
    testMat(0,0) = 1;
    testMat(1,1) = 1;
    testMat(2,2) = 1;
    testMat(3,3) = 1;
    return testMat;
}

//Uses ZYX convention for rotation
Matrix4 Matrix4::fullRotMat(float alpha, float beta, float gamma){
    Matrix4 rotMat;
    float cosA = std::cos(alpha);
    float sinA = std::sin(alpha);

    float cosB = std::cos(beta);
    float sinB = std::sin(beta);

    float cosG = std::cos(gamma);
    float sinG = std::sin(gamma);

    //First row
    rotMat(0,0) = cosB*cosG;
    rotMat(0,1) = -cosA*sinG + sinA*sinB*cosG;
    rotMat(0,2) = sinA*sinG + cosA*sinB*cosG;

    //Second row
    rotMat(1,0) = cosB*sinG;
    rotMat(1,1) = cosA*cosG + sinA*sinB*sinG;
    rotMat(1,2) = -sinA*cosG + cosA*sinB*sinG;

    //Third row
    rotMat(2,0) = -sinB;
    rotMat(2,1) = sinA*cosB;
    rotMat(2,2) = cosA*cosB;

    //Fourth row
    rotMat(3,3) = 1;

    return rotMat;
}

//Fills values along diagonal only
Matrix4 Matrix4::scaleMat(float scaleX, float scaleY, float scaleZ){
    Matrix4 scaleMat;
    scaleMat(0,0) = scaleX;
    scaleMat(1,1) = scaleY;
    scaleMat(2,2) = scaleZ;
    scaleMat(3,3) = 1;
    return scaleMat;
}

//Could porbably be combined with scale mat
Matrix4 Matrix4::translateMat(float dx, float dy, float dz){
    Matrix4 transMat;
    transMat(0,0) = 1;
    transMat(1,1) = 1;
    transMat(2,2) = 1;
    transMat(0,3) = dx;
    transMat(1,3) = dy;
    transMat(2,3) = dz;
    transMat(3,3) = 1;
    return transMat;
}
//Multiplication order is very important
// translation *(rotation*(scaling)) to avoid unexpected behaviour
Matrix4 Matrix4::transformMatrix(TransformParameters transform){
    Matrix4 rotMatrix = Matrix4::fullRotMat(transform.rotation.x,
                         transform.rotation.y, transform.rotation.z);
    Matrix4 scaleMatrix = Matrix4::scaleMat(transform.scaling.x,
                         transform.scaling.y, transform.scaling.z);
    Matrix4 translationMatrix = Matrix4::translateMat(transform.translation.x,
                         transform.translation.y, transform.translation.z);

    Matrix4 temp = (rotMatrix*scaleMatrix);
    return  translationMatrix*(temp);
}

//Moves the whole world to place the origin at the current camera position
//And wrt its direction
Matrix4 Matrix4::lookAt(Vector3f& position, Vector3f& target, Vector3f& temp){
    //Gram–Schmidt process
    Vector3f forward = (position - target).normalized();
    Vector3f side    = (temp.crossProduct(forward)).normalized();
    Vector3f up      = forward.crossProduct(side);

    //We will now build the inverse transform from the world position to the camera
    //The idea is that we don't care where the camera is, we only care about what
    //transformation would put the origin at the camera world space position
    //With the z axis behind the camera.
    Matrix4 worldToCam;
    
    //First row
    worldToCam(0,0) = side.x;
    worldToCam(0,1) = side.y;
    worldToCam(0,2) = side.z;
    worldToCam(0,3) = -side.dotProduct(position);

    //Second row
    worldToCam(1,0) = up.x;
    worldToCam(1,1) = up.y;
    worldToCam(1,2) = up.z;
    worldToCam(1,3) = -up.dotProduct(position);

    //Third row
    worldToCam(2,0) = forward.x;
    worldToCam(2,1) = forward.y;
    worldToCam(2,2) = forward.z;
    worldToCam(2,3) = -forward.dotProduct(position);

    //Fourth row
    worldToCam(3,3) = 1;

    return worldToCam;
}
//Assumes a symmetrical frustrum with euqal fov for horizontal and vertical
//Also uses the inverse z trick to in theory improve zbuffer precision
Matrix4 Matrix4::projectionMatrix(float fov, float AR, float near, float far){
    Matrix4 projectionMat;
    float tanHalfFOVInverse   =  1/tan( (fov/2) * (M_PI / 180) );
    
    //First Row
    projectionMat(0,0) = tanHalfFOVInverse; // near/right

    //Second row
    projectionMat(1,1) = AR * tanHalfFOVInverse; //near / top (top = right /AR)

    //Third row (Calculated for 1- 0) Inverse z buffer black magic
    projectionMat(2,2) =  (near) / (far - near);
    projectionMat(2,3) =  (far * near) / (far - near);
    
    //Fourth row
    projectionMat(3,2) = -1;
    
    return projectionMat;
}

//Transposed to get world-tangent transform
//You can transpose it instead of inverting it because it's orthogonal
Matrix4 Matrix4::TBNMatrix(const Vector3f &tangent, const Vector3f &biTangent, const Vector3f &normal){
    Matrix4 tangentMat;

    //First 
    tangentMat(0,0) = tangent.x;
    tangentMat(0,1) = biTangent.x;
    tangentMat(0,2) = normal.x;

    //Second row
    tangentMat(1,0) = tangent.y;
    tangentMat(1,1) = biTangent.y;
    tangentMat(1,2) = normal.y;

    //third row
    tangentMat(2,0) = tangent.z;
    tangentMat(2,1) = biTangent.z;
    tangentMat(2,2) = normal.z;

    //fourth row
    tangentMat(3,3) = 1.0;

    return tangentMat.transpose();

}

void Matrix4::print(){
    int n = 4;
    for(int rows = 0; rows < n; ++rows){
        for(int cols = 0; cols < n; ++cols){
            float val = (*this)(rows,cols) ;
            printf("%f\t",val);
        }
        printf("\n");
    }
    printf("\n");
}