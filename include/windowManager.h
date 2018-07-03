#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "SDL.h"

class WindowManager{

    public:
    const static int SCREEN_WIDTH  = 640; //640
    const static int SCREEN_HEIGHT = 480; //480
    WindowManager();

    ~WindowManager();

    bool startUp();

    bool startSDL();

    bool createWindow();

    SDL_Window* getWindow();

    void shutDown();

    void wait();

    private:
    SDL_Window  *mainWindow;


};

#endif