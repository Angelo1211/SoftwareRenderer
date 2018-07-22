#ifndef SHADER_H
#define SHADER_H

#include "vector3D.h"
#include "matrix.h"
#include "rasterizer.h"
#include <array>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3f vertex(Vector3f &vertex, Matrix4 &MVP, Vector3f &normals, Vector3f &light, int i) = 0;
    virtual bool fragment(Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    float varIntensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Matrix4 &MVP, Vector3f &normals, Vector3f &light, int index) override{
        varIntensity = std::max(0.0f,normals.dotProduct(light));
        return MVP.matMultVec(vertex); //Transforms verts into projected space
    }

    bool fragment(Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        depth = bari.dotProduct(zVerts);
        color = rgb*varIntensity;
        return false;
    }

};

//More Complex shader that calculates a per-vertex intensity and interpolates 
//through the fragments of the triangle
struct GouraudShader : public IShader {
    Vector3f varying_intensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Matrix4 &MVP, Vector3f &normals, Vector3f &light, int index) override{
        varying_intensity.data[index] = std::max(0.0f, normals.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        float intensity = bari.dotProduct(varying_intensity);
        color = rgb * intensity;
        depth = bari.dotProduct(zVerts);
        return false;
    }

};


#endif