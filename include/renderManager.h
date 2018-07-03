#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SDL.h"
#include "windowManager.h"
#include "texture.h"
#include "rasterizer.h"
#include "canvas.h"
#include "model.h"

class RenderManager{

    public:
        RenderManager();

        ~RenderManager();

        bool startUp(WindowManager windowManager);

        bool createRenderer(SDL_Window * mainWindow);

        bool createScreenTexture();

        void clearScreen();

        void updateScreen();

        bool createCanvas();

        void render(Model *models);

        void shutDown();

    private:
        SDL_Renderer *mainRenderer;
        Texture screenTexture;
        Canvas *mainCanvas;
        Rasterizer *raster;
    
};

#endif