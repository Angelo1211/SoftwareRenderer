#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SDL.h"
#include "windowManager.h"
#include "texture.h"

class RenderManager{

    public:
        static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGBA8888;
        RenderManager();

        ~RenderManager();

        bool startUp(WindowManager windowManager);

        bool createRenderer(SDL_Window * mainWindow);

        bool createScreenTexture();

        void clearScreen();

        void updateScreen();

        bool createBuffer();

        void render();

        void createPixelPattern();

        void shutDown();

    private:
        int pixelCount;
        Uint32 * buffer1;
        SDL_Renderer  *mainRenderer;
        Texture screenTexture;
    
};

#endif