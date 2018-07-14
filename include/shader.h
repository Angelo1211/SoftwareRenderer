#ifndef SHADER_H
#define SHADER_H

#include "vector3.h"
#include "matrix.h"
#include "rasterizer.h"
#include <array>

struct IShader {
    virtual ~IShader() {};
    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, float intens) = 0;
    virtual bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) = 0;
};

struct FlatShader : public IShader {
    float intensity;
    Vector3 rgb{255,255,255};

    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, float intens){
        intensity = intens;
        return MVP.matMultVec(vertex);
    }

    virtual bool fragment(Vector3 &lambdas, Vector3 &color, float &depth, Vector3 &zVerts){
        color = rgb*intensity;
        depth = lambdas.dotProduct(zVerts);
        return false;
    }

};

struct GourardShader : public IShader {
    float intensity;
    Vector3 rgb{255,255,255};

    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, float intens){
        intensity = intens;
        return MVP.matMultVec(vertex);
    }

    virtual bool fragment(Vector3 &lambdas, Vector3 &color, float &depth, Vector3 &zVerts){
        color = rgb*intensity;
        depth = lambdas.dotProduct(zVerts);
        return false;
    }

};


#endif