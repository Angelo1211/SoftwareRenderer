#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "vector3D.h"



class Texture{
    public:
        Texture(std::string path, std::string type); 

        ~Texture();

        Vector3f getPixelVal(float u, float v);

    private:
        float *pixelData;
        int width, height, channels;
};

#endif