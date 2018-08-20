#ifndef SHADER_H
#define SHADER_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-12
// PURPOSE      : Emulate modern programmable vertex and fragment shaders. Allow texture
//                reading and full Physically based rendering models. 
// ===============================
// SPECIAL NOTES: I kpet the older shaders that I wrote while working towards
// building the final PBR model because I thought it would be nice to see the progression
// Although using pure interface classes would seem to incur a perforamnce
// penalty through pointer chasing I did not measure it through profiling.
// ===============================

//Headers
#include "vector3D.h"
#include "matrix.h"
#include "texture.h"
#include <math.h>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                            const Vector3f &textureVals,const Vector3f &tangent,
                            int index,  const Vector3f &light = Vector3f{1,1,1}) = 0;
    virtual Vector3f fragment(float u, float v) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    Matrix4 MVP, MV;
    float varIntensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                    const Vector3f &textureVals,const Vector3f &tangent,
                    int index, const Vector3f &light) override
    {
        varIntensity = std::max(0.0f,normal.dotProduct(light));
        return MVP.matMultVec(vertex); //Transforms verts into projected space
    }

    Vector3f fragment(float u, float v) override{
        return rgb*varIntensity;
    }

};

//More Complex shader that calculates a per-vertex intensity and interpolates 
//through the fragments of the triangle
struct GouraudShader : public IShader {
    //Per object data
    Matrix4 MVP, MV, V, N;
    Vector3f lightColor1{1,1,1}, lightColor2{0,0,1}, lightColor3{1,1,1};
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.5, spec = 0;
    Vector3f rgb{255,255,255};

    //Per vertex data
    Vector3f varying_diffuse, varying_specular, reflectDir, viewDir, lightDir, correctNormal;

    //Per pixel data
    Vector3f ambient, diffuse, specular;

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                    const Vector3f &textureVals,const Vector3f &tangent,
                    int index, const Vector3f &light) override
    {   
        //Vertex attributes
        correctNormal = N.matMultDir(normal).normalized();
        lightDir = V.matMultDir(light).normalized();
        reflectDir = Vector3f::reflect(-lightDir, correctNormal);
        viewDir = MV.matMultVec(vertex).normalized();
        
        //Values to be interpolated
        varying_specular.data[index] = std::pow( std::max( -viewDir.dotProduct(reflectDir), 0.0f), 32.0f);
        varying_diffuse.data[index] = std::max(0.0f, (correctNormal).dotProduct(-lightDir));

        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolating
        diffStrength = varying_diffuse.x + u*(varying_diffuse.y - varying_diffuse.x) + v*(varying_diffuse.z - varying_diffuse.x);
        spec = varying_specular.x + u*(varying_specular.y - varying_specular.x) + v*(varying_specular.z - varying_specular.x);

        //Phong reflection model
        ambient = lightColor1 * ambientStrength;
        diffuse = lightColor2 * diffStrength;
        specular = lightColor3 * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
    }

};

//Even more complex shader that interpolates normals and calculates intensities per fragment instead
//instead of per vertex.
struct PhongShader : public IShader {
    //Per object data
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.9, spec = 0;
    Vector3f lightColor{0,0.1,1},lightColorSpec{1,1,1};
    Vector3f rgb{255,255,255};

    //Per vertex data
    Vector3f normals[3], viewDir[3];
    Vector3f varying_diffuse, varying_specular, reflectDir, lightDir;

    //Per pixel data
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir;


    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                    const Vector3f &textureVals,const Vector3f &tangent,
                    int index, const Vector3f &light) override
    {
        //Vertex attributes
        normals[index] = N.matMultDir(normal).normalized();
        viewDir[index] = MV.matMultVec(vertex).normalized();
        lightDir = V.matMultDir(light).normalized();

        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpNormal = normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v;
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diffStrength = std::max(0.0f, (interpNormal.normalized()).dotProduct(lightDir));
        diffuse = lightColor * diffStrength;
        
        //Specular
        reflectDir = Vector3f::reflect(-lightDir, interpNormal);
        spec = std::pow( std::max( (-interpViewDir.normalized()).dotProduct(reflectDir), 0.0f), 50.0f);
        specular = lightColorSpec * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
    }

};

//Optimized version of Phong shader that uses a half angle instead of individual reflection
//angles
struct BlinnPhongShader : public IShader {
    //Per object data
    Texture *albedoT;
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength=1 , specularStrength= 0.5;
    Vector3f lightColor{1,1,1};

    //Per vertex data
    Vector3f normals[3], viewDir[3], UV[3];
    float diff, spec, shininess = 128;
    
    //Per fragment data
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir, interpUV;
    Vector3f halfwayDir, lightDir;
    Vector3f interpCol, white{255,255,255};

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                    const Vector3f &textureVals,const Vector3f &tangent,
                    int index, const Vector3f &light) override{
        normals[index] = N.matMultDir(normal).normalized();
        UV[index] = textureVals;
        viewDir[index] = MV.matMultVec(vertex).normalized();
        lightDir = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpNormal = (normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v).normalized();
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        interpUV = UV[0] + (UV[1] - UV[0])* u + (UV[2] - UV[0]) * v;
        //Albedo
        interpCol = albedoT->getPixelVal(interpUV.x, interpUV.y);

        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diff = std::max(0.0f, interpNormal.dotProduct(lightDir));
        diffuse = lightColor * diff * diffStrength;
        
        //Specular
        halfwayDir = (lightDir - interpViewDir).normalized();
        spec = std::pow(std::max(0.0f, interpNormal.dotProduct(halfwayDir)), shininess);
        specular = lightColor * spec * specularStrength;

        return (ambient + diffuse) * interpCol + specular * white;
    }

};

// Shader that uses texture mapping extensively
struct TextureMapShader : public IShader {
    //Variables set per model
    Texture *albedoT, *normalT, *ambientOT;
    Matrix4 MVP, MV, V, M, N;
    Vector3f cameraPos;

    //Light Variables
    Vector3f lightColor{1,1,1}, white{1,1,1};
    float ambientStrength = 0.1, diffStrength = 0.9, specularStrength = 0.8;
    float diff, spec, shininess = 128;
    Vector3f lightDir[3];

    //Variables set per vertex
    Vector3f viewDir[3], texCoords[3];
    Vector3f normal_WS, tangent_WS, biTangent_WS;
    Matrix4 TBN;
    
    //Interpolated variables
    Vector3f interpCoords, interpLightDir, interpNormal,
             interpViewDir, interpCol, interpAO;

    //Per fragment
    Vector3f ambient, diffuse, specular ;
    Vector3f halfwayDir;

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                    const Vector3f &textureVals, const Vector3f &tangent,
                    int index, const Vector3f &light) override{
        //Creating TBN matrix
        normal_WS     = N.matMultDir(normal).normalized();
        tangent_WS    = N.matMultDir(tangent).normalized();
        biTangent_WS  = normal_WS.crossProduct(tangent_WS);
        TBN = Matrix4::TBNMatrix(tangent_WS, biTangent_WS, normal_WS);
        
        //Getting UV coordinates for use in both albedo and normal textures
        texCoords[index] = textureVals;

        //Passing all lighting related data to tangent space
        lightDir[index]  = TBN.matMultVec(light);
        viewDir[index]   = TBN.matMultVec(cameraPos - M.matMultVec(vertex));
        
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated attributes
        interpCoords   = texCoords[0] + (texCoords[1] - texCoords[0])* u + (texCoords[2] - texCoords[0]) * v;
        interpLightDir = lightDir[0] + (lightDir[1] - lightDir[0])* u + (lightDir[2] - lightDir[0]) * v;
        interpViewDir  = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;

        //Reading albedo and normal data from textures
        interpCol    = albedoT->getPixelVal(interpCoords.x, interpCoords.y);
        interpAO     = ambientOT->getIntensityVal(interpCoords.x, interpCoords.y);
        interpNormal = normalT->getPixelVal(interpCoords.x, interpCoords.y);
        interpNormal = interpNormal.normalized();

        //Ambient 
        ambient = lightColor * ambientStrength * interpAO;

        //Diffuse
        diff = std::max(0.0f, interpNormal.dotProduct(interpLightDir));
        diffuse = lightColor * diff * diffStrength;
        
        //Specular
        halfwayDir = (interpLightDir + interpViewDir).normalized();
        spec = std::pow(std::max(0.0f, interpNormal.dotProduct(halfwayDir)), shininess);
        specular = lightColor * spec * specularStrength;

        return (ambient + diffuse) * interpCol + specular * white;
    }

};

// Shader that uses texture mapping and a PBR approach for shading
// Uses a cook-torrance BRDF for direct light sources. 
struct PBRShader : public IShader {
    //Variables set per model
    Texture *albedoT, *normalT, *ambientOT, *roughT, *metalT;
    Matrix4 MVP, MV, V, M, N;
    Vector3f cameraPos;

    //Light Variables
    Vector3f F0{0.04, 0.04, 0.04}, F0corrected; //Default value dielectric
    Vector3f *lightDirVal, *lightCol, *lightPos;
    float nDotL, nDotV, ambientInt = 0.01; 
    int numLights;

    //Variables set per vertex
    Vector3f viewDir[3], texCoords[3];
    Vector3f normal_WS, tangent_WS, biTangent_WS;
    Matrix4 TBN;
    
    //Interpolated variables
    Vector3f interpCoords, interpNormal, interpViewDir, interpCol;

    //Per fragment
    Vector3f radianceOut, ambient;
    float interpRough, interpAO, interpMetal;
    float uTexture, vTexture, intPart;

    //BRDF functions
    Vector3f fresnelSchlick(float cosTheta, Vector3f &fresnel0 ){
        float invcCosTheta = 1.0 - cosTheta;
        return fresnel0 + (Vector3f(1.0)- fresnel0) * (invcCosTheta * invcCosTheta * invcCosTheta * invcCosTheta * invcCosTheta);
    }
    float distributionGGX(Vector3f normal, Vector3f halfway, float roughness){
        float a      = roughness*roughness;
        float a2     = a*a;
        float NdotH  = std::max(normal.dotProduct(halfway), 0.0f);
        float NdotH2 = NdotH*NdotH;
        
        float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
        denom =  M_1_PIf32/ (denom * denom);
        
        return a2 * denom;
    }
    float GeometrySchlickGGX(float Ndot, float roughness){
        float r = (roughness + 1.0f); 
        float k = (r*r) / 8.0f; //Only useful for direct lighting must be changed in ibr
        float denom = 1.0f / (Ndot * (1.0f- k) + k);
        
        return Ndot * denom;
    }
    float GeometrySmith(float roughness, float nDL, float nDV){
        return GeometrySchlickGGX(nDL, roughness) * GeometrySchlickGGX(nDV, roughness);
    }

    //Vertex shader
    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,
                     const Vector3f &textureVals, const Vector3f &tangent,
                    int index, const Vector3f &light = Vector3f{1,1,1}) override
    {             
        //Creating TBN matrix
        normal_WS     = N.matMultDir(normal).normalized();
        tangent_WS    = N.matMultDir(tangent).normalized();
        biTangent_WS  = normal_WS.crossProduct(tangent_WS);
        TBN = Matrix4::TBNMatrix(tangent_WS, biTangent_WS, normal_WS);
        
        //Getting UV coordinates for use in all textures
        texCoords[index] = textureVals;

        //Passing all lighting related data to tangent space
        for(int lIndex = 0; lIndex < numLights; ++lIndex){
            int indc2 = (lIndex*3) + index;
            lightDirVal[indc2]  = TBN.matMultDir(lightPos[lIndex]);
        }
        viewDir[index]   = TBN.matMultDir(cameraPos - M.matMultVec(vertex));
        
        return MVP.matMultVec(vertex);
    }

    //Fragment shader
    Vector3f fragment(float u, float v) override{
        //Interpolated attributes
        interpCoords   = texCoords[0] + (texCoords[1] - texCoords[0])* u + (texCoords[2] - texCoords[0]) * v;
        interpViewDir  = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;

        //Correcting UV's for tiling
        uTexture = std::modf(interpCoords.x, &intPart);
        vTexture = std::modf(interpCoords.y, &intPart);

        //Reading data from textures for use in lighting calculations
        interpCol     = albedoT->getPixelVal(uTexture, vTexture);
        interpAO      = ambientOT->getIntensityVal(uTexture, vTexture);
        interpRough   = roughT->getIntensityVal(uTexture, vTexture);;
        interpMetal   = metalT->getIntensityVal(uTexture, vTexture);
        interpNormal  = normalT->getPixelVal(uTexture, vTexture);
        interpNormal  = interpNormal.normalized();
        interpViewDir = interpViewDir.normalized();

        //Varying f0 based on metallicness of surface
        float invMetal = (1.0f-interpMetal);
        F0corrected = (F0 * invMetal) + (interpCol * interpMetal);
        nDotV = std::max(interpNormal.dotProduct(interpViewDir), 0.0f);

        //Setting up Direct Lighting variables
        const int maxLights = numLights;

        //Fresnel, normal distribution function and geometry occlusion 
        Vector3f F[maxLights];
        float  NDF[maxLights];
        float  G[maxLights];
        
        //Storing in array for vectorizing
        Vector3f radianceLights[maxLights];
        Vector3f interpLightDir[maxLights];
        Vector3f halfwayDir[maxLights];
        float  nDotL[maxLights];
        Vector3f numerator[maxLights];
        float  invDenominator[maxLights];
        Vector3f specular[maxLights];
        Vector3f kD[maxLights];
        
        //Interpolating each light direction for every light
        int val;
        for(int i = 0; i < maxLights; ++i ){
            val = i*3;
            interpLightDir[i] = (lightDirVal[val] +  (lightDirVal[val + 1] - lightDirVal[val])* u +  (lightDirVal[val + 2] - lightDirVal[val]) * v).normalized();
        }

        //Per light illumination calculations that can be simdified
        //Currently uses widest SIMD array to perform all light iterations in one trip
        //Which I believe leaves some extra 
        #pragma omp simd
        for(int light = 0; light < maxLights; ++light ){
            halfwayDir[light] = (interpLightDir[light] + interpViewDir);
            halfwayDir[light] = halfwayDir[light].normalized();
            nDotL[light] = std::fmax(interpNormal.dotProduct(interpLightDir[light]), 0.0f);

            //No problem vectorizing these functions because they are inlined by the compiler
            //And also only consist of math operations to the vectors
            F[light]     = fresnelSchlick(std::fmax(halfwayDir[light].dotProduct(interpViewDir), 0.0f), F0corrected);
            NDF[light]   = distributionGGX(interpNormal, halfwayDir[light], interpRough); 
            G[light]     = GeometrySmith(interpRough, nDotL[light] , nDotV);

            numerator[light] = F[light] * G[light]*NDF[light];
            invDenominator[light]  = 1.0f / std::fmax(4.0f * (nDotL[light] * nDotV), 0.001f);
            specular[light]  = numerator[light] * invDenominator[light];

            //kd is 1 - kf which is the stuff to the right of the vecotr 
            kD[light] = (Vector3f(1.0f) - F[light])*invMetal;

            //The rendering equation result for a given light
            radianceLights[light] = (kD[light] * (interpCol * (M_1_PIf32)) + specular[light] ) * nDotL[light] * lightCol[light];
        }

        //Summing up all radiance values since SIMD won't work if I do this within the
        //previous loop
        radianceOut.zero();
        for(int i = 0; i < maxLights; ++i) {
            radianceOut += radianceLights[i];
        }

        //Simplistic ambient term
        ambient =  interpCol * (ambientInt * interpAO);

        return ambient + radianceOut;
    }
    
};
#endif