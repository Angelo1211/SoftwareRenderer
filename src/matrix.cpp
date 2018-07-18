#include "matrix.h"
#include <math.h>

Vector3f Matrix4::matMultVec(Vector3f &vec){
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
    
    //Division is expensive, only do it if you need it
    //Also here is where perspective divide happens!
    // if(w2 != 1){
    //     newVec.x /= w2;
    //     newVec.y /= w2;
    //     newVec.z /= w2;
    // }
    newVec.w = w2;

    return newVec;
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

Matrix4 Matrix4::scaleMat(float scaleX, float scaleY, float scaleZ){
    Matrix4 scaleMat;
    scaleMat(0,0) = scaleX;
    scaleMat(1,1) = scaleY;
    scaleMat(2,2) = scaleZ;
    scaleMat(3,3) = 1;
    return scaleMat;
}

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

Matrix4 Matrix4::lookAt(Vector3f& position, Vector3f& target, Vector3f& temp){
    //Gram–Schmidt process
    Vector3f forward = (position - target).normalized();
    Vector3f side    = (temp.crossProduct(forward)).normalized();
    Vector3f up      = forward.crossProduct(side);

    //We will now build the inverse transform from the world position to the camera
    //The idea is that we don't care where the camera is, we only care about what
    //transformation would put the origin at the camera world space position
    //With the z axis behind the camera.
    //I bet you this will be really confusing in a couple of weeks
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

Matrix4 Matrix4::projectionMatrix(float fov, float AR, float near, float far){
    Matrix4 projectionMat;
    float tanHalfFOVInverse   =  1/tan( (fov/2) * (M_PI / 180) );
    
    //First Row
    projectionMat(0,0) = tanHalfFOVInverse; // near/right

    //Second row
    projectionMat(1,1) = AR * tanHalfFOVInverse; //near / top (top = right /AR)

    //Third row (Calculated for 1- 0)
    projectionMat(2,2) =  (near) / (far - near);
    projectionMat(2,3) =  (far * near) / (far - near);
    
    //Fourth row
    projectionMat(3,2) = -1;
    
    return projectionMat;
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