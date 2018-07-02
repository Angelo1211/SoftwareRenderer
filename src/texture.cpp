#include "texture.h"
#include "windowManager.h"

Texture::Texture(){
    mTexture = NULL;
    mWidth   = 0;
    mHeight  = 0;
    mPitch   = 0;
}

Texture::~Texture(){
    free();
}

void Texture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture (mTexture);
        mWidth  = 0;
        mHeight = 0;
        mPitch  = 0; 
    }
}

bool Texture::createBlank(SDL_Renderer  *renderer, int width, int height ){
    mWidth  = width;
    mHeight = height;
    mPitch  = width * sizeof(Uint32);
    mTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC, mWidth, mHeight);

    return mTexture != NULL;
}

void Texture::updateTexture(Uint32 * pixels){
    SDL_UpdateTexture(mTexture, NULL, pixels, mPitch);
}

void Texture::renderToScreen(SDL_Renderer * renderer){
    SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, mTexture, NULL , NULL);
}