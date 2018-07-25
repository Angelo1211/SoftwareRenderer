#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "vector3D.h"

class Texture{
    public:
        Texture(std::string path); 

        Vector3f getPixelVal(float u, float v);

    private:
        unsigned char *pixelData;
        int width, height, channels, origin;
};

#endif