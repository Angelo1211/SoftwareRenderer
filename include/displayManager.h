#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "SDL.h"
#include "buffer.h"
#include "texture.h"

class DisplayManager{

    public:
        const static int SCREEN_WIDTH  = 640; //640
        const static int SCREEN_HEIGHT = 480; //480

        //Dummy Constructor / Destructor
        DisplayManager();
        ~DisplayManager();

        //Initializes SDL context and creates window according to values above 
        bool startUp();
        void shutDown();

        //Clear screens to black
        void clear();

        //Swaps the pixel buffer with the texture buffer and draws to screen
        void draw(Buffer<Uint32> *pixelBuffer);

    private:
        bool startSDL();
        bool createWindow();
        bool createSDLRenderer();
        bool createScreenTexture();

        Texture      screenTexture;
        SDL_Surface  *surface;
        SDL_Renderer *SDLRenderer;
        SDL_Window   *mainWindow;
};

#endif