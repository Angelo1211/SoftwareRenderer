#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "SDL.h"
#include "windowManager.h"
#include "texture.h"
#include "rasterizer.h"
#include "canvas.h"
#include "model.h"

class RenderManager {

    public:
        RenderManager();

        ~RenderManager();

        bool startUp(WindowManager windowManager);        

        void render(Model *models, Matrix4 &mat);

        void shutDown();

    private:
        //Init methods
        bool createRenderer(SDL_Window * mainWindow);
        bool createCanvas();
        bool createScreenTexture();
        void createProjectionMatrix();

        //Rendering pipeline stuff
        void clearScreen();
        void updateScreen();

        //Per vertex stuff
        

        SDL_Renderer *mainRenderer;
        Texture screenTexture;
        Canvas *mainCanvas;
        Rasterizer *raster;
        Matrix4 projectionMatrix;

};

#endif