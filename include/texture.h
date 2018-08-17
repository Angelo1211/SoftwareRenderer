#ifndef TEXTURE_H
#define TEXTURE_H

// ===============================
// AUTHOR       : Angel Ortiz (angelo12 AT vt DOT edu)
// CREATE DATE  : 2018-07-02
// PURPOSE      : To store texture data for retrieval during the pixel shader phase.
//                it also has to be capable of storing all the major kinds of textures
//                that are used in a physically based renderer.
// ===============================
// SPECIAL NOTES: The two separate methods to get pixel values are not good style,
// and should probably be changed. The reason they're there is because of the different
// return statement for each. Should probably become either different classes or just
// a more general solution.
// ===============================

//Headers
#include <string>
#include "vector3D.h"

class Texture{
    public:
        Texture(std::string path, std::string type); 
        ~Texture();

        Vector3f getPixelVal(float u, float v);
        float getIntensityVal(float u, float v);

    private:
        float *pixelData;
        int width, height, channels, tileW = 32, tileH = 32, widthInTiles;

        //Currently disabled after tiling has been implemented
        int bilinearFiltering(float u, float v);

        //Reorganizes pixel data into a more cache friendly form
        void tileData();
};

#endif