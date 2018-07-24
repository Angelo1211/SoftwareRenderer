#ifndef SHADER_H
#define SHADER_H

#include "vector3D.h"
#include "matrix.h"
#include <array>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3f vertex(Vector3f &vertex, Vector3f &normals, Vector3f &light, int i) = 0;
    virtual bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    Matrix4 MVP, MV;
    float varIntensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normals, Vector3f &light, int index) override{
        varIntensity = std::max(0.0f,normals.dotProduct(light));
        return MVP.matMultVec(vertex); //Transforms verts into projected space
    }

    bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        depth = bari.dotProduct(zVerts);
        color = rgb*varIntensity;
        return false;
    }

};

//More Complex shader that calculates a per-vertex intensity and interpolates 
//through the fragments of the triangle
struct GouraudShader : public IShader {
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.5, spec = 0;
    Vector3f ambient, diffuse, specular;
    Vector3f lightColor1{1,1,1}, lightColor2{0,0,1}, lightColor3{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, viewDir, light2;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &light, int index) override{
        normal = N.matMultDir(normal).normalized();
        light2 = V.matMultDir(light).normalized();
        reflectDir = Vector3f::reflect(-light2, normal);
        viewDir = MV.matMultVec(vertex).normalized();
        varying_specular.data[index] = std::pow( std::max( -viewDir.dotProduct(reflectDir), 0.0f), 32.0f);
        varying_diffuse.data[index] = std::max(0.0f, (normal).dotProduct(light2));
        return MVP.matMultVec(vertex);
    }

    bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        ambient = lightColor1 * ambientStrength;

        diffStrength = bari.dotProduct(varying_diffuse);
        diffuse = lightColor2 * diffStrength;

        spec = bari.dotProduct(varying_specular);
        specular = lightColor3 * (specularStrength * spec);

        color = (ambient + diffuse + specular) * rgb;

        depth = bari.dotProduct(zVerts);
        return false;
    }

};
//Even more complex shader that interpolates normals and calculates intensities per fragment instead
//of per normals. Uses half angle optimization.
struct BlinnPhongShader : public IShader {
    Matrix4 MVP, MV;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.5, spec = 0;
    Vector3f ambient, diffuse, specular;
    Vector3f lightColor1{1,0,0}, lightColor2{0,0,1}, lightColor3{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, viewDir;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &light, int index) override{
        reflectDir = Vector3f::reflect(-light, normal);
        viewDir = MV.matMultVec(vertex);
        varying_specular.data[index] = std::pow( std::max(viewDir.dotProduct(reflectDir), 0.0f), 32.0f);
        varying_diffuse.data[index] = std::max(0.0f, normal.dotProduct(light));
        return MVP.matMultVec(vertex);
    }

    bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        ambient = lightColor1 * ambientStrength;
        
        diffStrength = bari.dotProduct(varying_diffuse);
        diffuse = lightColor2 * diffStrength;

        spec = bari.dotProduct(varying_specular);
        specular = lightColor3 * (specularStrength * spec);

        color = (ambient + diffuse + specular) * rgb;

        depth = bari.dotProduct(zVerts);
        return false;
    }

};


#endif