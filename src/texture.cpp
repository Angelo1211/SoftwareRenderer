#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path, std::string type){
    stbi_set_flip_vertically_on_load(true);  
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data){
        if(type == "RGB"){
            pixelData = new float[width*height*channels];
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = (float)data[i] * (1/256.0f);
            }
        }
        else if (type == "XYZ"){
            pixelData = new float[width*height*channels];
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = (float)data[i] * (2/256.0f) - 1.0f;
            }
        }
        else if (type == "BW"){
            pixelData = new float[width*height*channels];
            for(int i = 0; i < width*height*channels; ++i){
                pixelData[i] = (float)data[i] * (1/256.0f);
            }
        }
        else{
            printf("Error unrecognized texture format type.\n");
        }
    }
    else{
        printf("Failed to load texture at: %s\n",path.c_str());
    }
    stbi_image_free(data);
}

Texture::~Texture(){
    delete [] pixelData;
}

Vector3f Texture::getPixelVal(float u, float v){
    int uInt = u * (width-1); 
    int vInt = v * (height-1);
    int index = (vInt*width + uInt)*channels;
    return Vector3f{pixelData[index], pixelData[index+1], pixelData[index+2]};
}

float Texture::getIntensityVal(float u, float v){
    int uInt = u * (width-1); 
    int vInt = v * (height-1);
    int index = (vInt*width + uInt);
    return pixelData[index];
}