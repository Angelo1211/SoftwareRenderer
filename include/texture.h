#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"

class Texture{

    public:
        Texture();

        ~Texture();
        
        void free();

        bool createBlank(SDL_Renderer * mainRenderer, int width, int height);

        void updateTexture(Uint32 * pixels);

        void renderToScreen(SDL_Renderer * mainRenderer);

    private:
        int mPitch;
        SDL_Texture* mTexture;

        //Dimensions
        int mWidth;
        int mHeight;

};

#endif