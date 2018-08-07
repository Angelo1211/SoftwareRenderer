#ifndef SHADER_H
#define SHADER_H

#include "vector3D.h"
#include "matrix.h"
#include "texture.h"
#include <array>

//Shader Interface for a class that emulates modern GPU fragment and vertex shaders
struct IShader {
    virtual ~IShader() {};
    virtual Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals,const Vector3f &tangent, const Vector3f &light, int index) = 0;
    virtual Vector3f fragment(float u, float v) = 0;
};

//Simplest shader. Calculates light intensity per triangle.
struct FlatShader : public IShader {
    Matrix4 MVP, MV;
    float varIntensity;
    Vector3f rgb{255,255,255};

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals,const Vector3f &tangent, const Vector3f &light, int index) override{
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
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength = 0, specularStrength = 0.5, spec = 0;
    Vector3f ambient, diffuse, specular;
    Vector3f lightColor1{1,1,1}, lightColor2{0,0,1}, lightColor3{1,1,1};
    Vector3f varying_diffuse, varying_specular, reflectDir, viewDir, light2, normal2;
    Vector3f rgb{255,255,255};

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals,const Vector3f &tangent, const Vector3f &light, int index) override{
        normal2 = N.matMultDir(normal).normalized();
        light2 = V.matMultDir(light).normalized();
        reflectDir = Vector3f::reflect(-light2, normal);
        viewDir = MV.matMultVec(vertex).normalized();
        varying_specular.data[index] = std::pow( std::max( -viewDir.dotProduct(reflectDir), 0.0f), 32.0f);
        varying_diffuse.data[index] = std::max(0.0f, (normal).dotProduct(-light2));
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        ambient = lightColor1 * ambientStrength;

        diffStrength = varying_diffuse.x + u*(varying_diffuse.y - varying_diffuse.x) + v*(varying_diffuse.z - varying_diffuse.x);
        diffuse = lightColor2 * diffStrength;

        spec = varying_specular.x + u*(varying_specular.y - varying_specular.x) + v*(varying_specular.z - varying_specular.x);
        specular = lightColor3 * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
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

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals,const Vector3f &tangent, const Vector3f &light, int index) override{
        normals[index] = N.matMultDir(normal).normalized();
        viewDir[index] = MV.matMultVec(vertex).normalized();
        light2 = V.matMultDir(light).normalized();
        return MVP.matMultVec(vertex);
    }

    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpNormal = normals[0] + (normals[1] - normals[0])* u + (normals[2] - normals[0]) * v;
        interpViewDir = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;
        //Ambient 
        ambient = lightColor * ambientStrength;

        //Diffuse
        diffStrength = std::max(0.0f, (interpNormal.normalized()).dotProduct(light2));
        diffuse = lightColor * diffStrength;
        
        //Specular
        reflectDir = Vector3f::reflect(-light2, interpNormal);
        spec = std::pow( std::max( (-interpViewDir.normalized()).dotProduct(reflectDir), 0.0f), 50.0f);
        specular = lightColorSpec * (specularStrength * spec);

        return (ambient + diffuse + specular) * rgb;
    }

};

//Optimized version of Phong shader that uses a half angle instead of individual reflection
//angles
struct BlinnPhongShader : public IShader {
    Texture *albedoT;
    Matrix4 MVP, MV, V, N;
    float ambientStrength = 0.05, diffStrength=1 , specularStrength= 0.5;
    float diff, spec, shininess = 128;
    Vector3f normals[3], viewDir[3], UV[3];
    Vector3f ambient, diffuse, specular, interpNormal, interpViewDir, interpUV;
    Vector3f lightColor{1,1,1};
    Vector3f halfwayDir, lightDir;
    Vector3f interpCol, white{255,255,255};

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals,const Vector3f &tangent, const Vector3f &light, int index) override{
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

    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals, const Vector3f &tangent, const Vector3f &light, int index) override{
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
        //Interpolated stuff
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
    float nDotL, nDotV, ambientInt = 0.01;
    int numLights;
    Vector3f *lightDirVal;
    Vector3f *lightCol;
    Vector3f *lightPos;

    //Variables set per vertex
    Vector3f viewDir[3], texCoords[3];
    Vector3f normal_WS, tangent_WS, biTangent_WS;
    Matrix4 TBN;
    
    //Interpolated variables
    Vector3f interpCoords, interpLightDir,
             interpNormal, interpViewDir,
             interpCol;

    //Per fragment
    Vector3f F, NDF, G ; //Fresnel, normal distribution function and geometry occlusion 
    Vector3f halfwayDir, radianceOut, ambient;
    Vector3f specular, kD, kS;
    float interpRough, interpAO, interpMetal;
    float u, v, intPart;

    //BRDF functions
    Vector3f fresnelSchlick(float cosTheta, Vector3f &fresnel0 ){
        return fresnel0 + (Vector3f(1.0)- fresnel0)* std::pow(1.0 - cosTheta, 5.0);
    }
    float distributionGGX(Vector3f normal, Vector3f halfway, float roughness){
        float a      = roughness*roughness;
        float a2     = a*a;
        float NdotH  = std::max(normal.dotProduct(halfway), 0.0f);
        float NdotH2 = NdotH*NdotH;
        
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = M_PI * denom * denom;
        
        return a2 / denom;
    }
    float GeometrySchlickGGX(float NdotV, float roughness){
        float r = (roughness + 1.0); 
        float k = (r*r) / 8.0; //Only useful for direct lighting must be changed in ibr
        float denom = NdotV * (1.0 - k) + k;
        
        return NdotV / denom;
    }
    float GeometrySmith(float roughness){
        return GeometrySchlickGGX(nDotL, roughness) * GeometrySchlickGGX(nDotV, roughness);
    }

    //Vertex shader
    Vector3f vertex(const Vector3f &vertex, const Vector3f &normal,const Vector3f &textureVals, const Vector3f &tangent, const Vector3f &n, int index) override{
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
            lightDirVal[indc2]  = TBN.matMultVec(lightPos[lIndex]);
        }
        viewDir[index]   = TBN.matMultVec(cameraPos - M.matMultVec(vertex));
        
        return MVP.matMultVec(vertex);
    }

    //Fragment shader
    Vector3f fragment(float u, float v) override{
        //Interpolated stuff
        interpCoords   = texCoords[0] + (texCoords[1] - texCoords[0])* u + (texCoords[2] - texCoords[0]) * v;
        interpViewDir  = viewDir[0] + (viewDir[1] - viewDir[0])* u + (viewDir[2] - viewDir[0]) * v;

        //Correcting UV's for tiling
        u = std::modf(interpCoords.x, &intPart);
        v = std::modf(interpCoords.y, &intPart);

        //Reading data from textures for use in lighting calculations
        interpCol    = albedoT->getPixelVal(u, v);
        interpAO     = ambientOT->getIntensityVal(u, v);
        interpRough  = roughT->getIntensityVal(u, v);;
        interpMetal  = metalT->getIntensityVal(u, v);
        interpNormal = normalT->getPixelVal(u, v);
        interpNormal = interpNormal.normalized();
        interpViewDir = interpViewDir.normalized();

        //Setting up Direct Lighting variables
        radianceOut.zero();
        for(int light = 0; light < numLights; ++light ){
            int val = light*3;
            //interpLightDir = lightColor[0];

            interpLightDir = lightDirVal[0] +  (lightDirVal[1] - lightDirVal[0])* u +  (lightDirVal[2] - lightDirVal[0]) * v;
            halfwayDir = (interpLightDir.normalized() + interpViewDir).normalized();
            nDotV = std::max(interpNormal.dotProduct(interpViewDir), 0.0f);
            nDotL = std::max(interpNormal.dotProduct(interpLightDir), 0.0f);
            F0corrected = (F0 * (1.0f-interpMetal)) + (interpCol * interpMetal);//Varying f0 based on metallicness of surface


            //We assume the only light in the scene is the sun so there is no attenuation
            
            //Setting up BRDF
            F   = fresnelSchlick(std::max(halfwayDir.dotProduct(interpViewDir), 0.0f), F0corrected);
            NDF = distributionGGX(interpNormal, halfwayDir, interpRough); 
            G   = GeometrySmith(interpRough);

            //Calculating specular component of BRDF
            Vector3f numerator = F * (G*NDF);
            float invDenominator  = 1 / std::max(4.0 * nDotL * nDotV, 0.001);
            specular  = numerator * invDenominator;

            //Calculating the full rendering equation for a single light
            kS = F;
            kD = (Vector3f(1.0) - kS);
            kD = kD * (1.0f - interpMetal); 
            radianceOut +=  ( (kD * (interpCol * (1/M_PI)) + specular ) * nDotL * lightCol[light]);
        }
        
        //Simplistic ambient term
        ambient =  interpCol * (ambientInt * interpAO);

        return ambient + radianceOut;
    }

};





#endif