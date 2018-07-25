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
//instead of per vertex.
struct PhongShader : public IShader {
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.9, spec = 0;
    Vector3f normals[3], viewDir[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir;
    Vector3f lightColor{0,0.1,1},lightColorSpec{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, light2;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &light, int index) override{
        normals[index] = N.matMultDir(normal).normalized();
        viewDir[index] = MV.matMultVec(vertex).normalized();
        light2 = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        //Interpolated stuff
        interpNormal  = (normals[0] * bari.x) + (normals[1] * bari.y)  + (normals[2] * bari.z);
        interpViewDir = (viewDir[0] * bari.x) + (viewDir[1] * bari.y)  + (viewDir[2] * bari.z);
        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diffStrength = std::max(0.0f, (interpNormal.normalized()).dotProduct(light2));
        diffuse = lightColor * diffStrength;
        
        //Specular
        reflectDir = Vector3f::reflect(-light2, interpNormal);
        spec = std::pow( std::max( (-interpViewDir.normalized()).dotProduct(reflectDir), 0.0f), 50.0f);
        specular = lightColorSpec * (specularStrength * spec);

        color = (ambient + diffuse + specular) * rgb;

        depth = bari.dotProduct(zVerts);
        return false;
    }

};

//Optimized version of Phong shader that uses a half angle instead of individual reflection
//angles
struct BlinnPhongShader : public IShader {
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength, spec,shininess = 128;
    Vector3f normals[3], viewDir[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir;
    Vector3f lightColor{0,0.1,1},lightColorSpec{1,1,1};
    Vector3f halfwayDir, lightDir;
    Vector3f rgb{255,255,255};

    Vector3f vertex(Vector3f &vertex, Vector3f &normal, Vector3f &light, int index) override{
        normals[index] = N.matMultDir(normal).normalized();
        viewDir[index] = MV.matMultVec(vertex).normalized();
        lightDir = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    bool fragment(const Vector3f &bari, Vector3f &color, float &depth, Vector3f &zVerts) override{
        //Interpolated stuff
        interpNormal  = ((normals[0] * bari.x) + (normals[1] * bari.y)  + (normals[2] * bari.z)).normalized();
        interpViewDir = (viewDir[0] * bari.x) + (viewDir[1] * bari.y)  + (viewDir[2] * bari.z);
        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diffStrength = std::max(0.0f, interpNormal.dotProduct(lightDir));
        diffuse = lightColor * diffStrength;
        
        //Specular
        halfwayDir = (lightDir -interpViewDir).normalized();
        spec = std::pow(std::max(0.0f, interpNormal.dotProduct(halfwayDir)), shininess);
        specular = lightColorSpec * spec;

        color = (ambient + diffuse + specular) * rgb;

        depth = bari.dotProduct(zVerts);
        return false;
    }

};


#endif