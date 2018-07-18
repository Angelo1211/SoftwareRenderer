#ifndef SHADER_H
#define SHADER_H

#include "vector3.h"
#include "matrix.h"
#include "rasterizer.h"
#include <array>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, Vector3 &normals, Vector3 &light, int i) = 0;
    virtual bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    float varIntensity;
    Vector3 rgb{255,255,255};

    Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, Vector3 &normals, Vector3 &light, int index) override{
        varIntensity = std::max(0.0f,normals.dotProduct(light));
        return MVP.matMultVec(vertex); //Transforms verts into projected space
    }

    bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) override{
        depth = bari.dotProduct(zVerts);
        color = rgb*varIntensity;
        return false;
    }

};

//More Complex shader that calculates a per-vertex intensity and interpolates 
//through the fragments of the triangle
struct GouraudShader : public IShader {
    Vector3 varying_intensity;
    Vector3 rgb{255,255,255};

    Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, Vector3 &normals, Vector3 &light, int index) override{
        varying_intensity.data[index] = std::max(0.0f, normals.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) override{
        float intensity = bari.y*varying_intensity.x + bari.z*varying_intensity.y + bari.x*varying_intensity.z;
        color = rgb * intensity;
        depth = bari.dotProduct(zVerts);
        return false;
    }

};


#endif