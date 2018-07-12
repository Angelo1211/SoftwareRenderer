#ifndef SHADER_H
#define SHADER_H

#include "vector3.h"
#include "matrix.h"

struct IShader {
    virtual ~IShader() {};
    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP) = 0;
    virtual bool fragment(Vector3 &vertex) = 0;
};

struct FlatShader : public IShader {

    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP){
        return MVP.matMultVec(vertex);
    }

    virtual bool fragment(Vector3 &barCoordinates){
        return false;
    }

};


#endif

//     //View matrix transform
//     Vector3 v0 = viewMatrix.matMultVec((*vertices)[f.x-1]); //-1 because .obj file starts face count
//     Vector3 v1 = viewMatrix.matMultVec((*vertices)[f.y-1]); // from 1. Should probably fix this 
//     Vector3 v2 = viewMatrix.matMultVec((*vertices)[f.z-1]); // At some point


//     // Projection matrix transformation
//     v0 = projectionMatrix.matMultVec(v0);
//     v1 = projectionMatrix.matMultVec(v1);
//     v2 = projectionMatrix.matMultVec(v2);