#ifndef SHADER_H
#define SHADER_H

#include "vector3.h"
#include "matrix.h"
#include "rasterizer.h"
#include <array>

struct IShader {
    virtual ~IShader() {};
    virtual Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, float intensity, Vector3 &normals, Vector3 &light, int i) = 0;
    virtual bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) = 0;
};

struct FlatShader : public IShader {
    float varIntensity;
    Vector3 rgb{255,255,255};

     Vector3 vertex(Vector3 &vertex, Matrix4 &MVP, float intensity, Vector3 &normals, Vector3 &light, int index) override{
        varIntensity = intensity;
        return MVP.matMultVec(vertex);
    }

     bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) override{
        color = rgb*varIntensity;
        depth = bari.dotProduct(zVerts);
        return false;
    }

};

struct GouraudShader : public IShader {
    Vector3 varying_intensity;
    Vector3 rgb{255,255,255};

    Vector3 vertex(Vector3 &vertex, Matrix4 &MVP,float intensity, Vector3 &normals, Vector3 &light, int index) override{
        //normals.print();
        varying_intensity.data[index] = std::max(0.0f, normals.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(Vector3 &bari, Vector3 &color, float &depth, Vector3 &zVerts) override{
        //varying_intensity.print();
        float intensity = varying_intensity.dotProduct(bari);
        color = rgb * intensity;
        depth = bari.dotProduct(zVerts);
        return false;
    }

};


#endif