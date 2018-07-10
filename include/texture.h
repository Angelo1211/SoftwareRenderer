#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"

//Needs another pass in the future, since it's so highly coupled with SDL
//Okay for now but FIX THIS
class Texture{

    public:
        Texture();

        ~Texture();
        
        void free();

        bool createBlank(SDL_Renderer * mainRenderer, int width, int height);

        void update(Uint32 * pixels);

        void renderToScreen(SDL_Renderer * mainRenderer);

    private:
        SDL_Texture* mTexture;

        //Dimensions
        int mWidth;
        int mHeight;
        int mPitch;

        //Only used to lock and unlock pixels, not  the actual pixel
        //values FIX THIS
        void* mTrash;

};

#endif