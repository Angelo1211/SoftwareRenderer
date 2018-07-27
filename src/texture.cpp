#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path){
    stbi_set_flip_vertically_on_load(true);  
    pixelData = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

Vector3f Texture::getPixelVal(float u, float v){
    int uInt = u * (width-1); 
    int vInt = v * (height-1);
    int index = (vInt*width + uInt)*3;
    return Vector3f{(float)pixelData[index], (float)pixelData[index+1], (float)pixelData[index+2]};
}