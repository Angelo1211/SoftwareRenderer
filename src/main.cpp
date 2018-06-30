#include <stdio.h>
#include "SDL.h"

//Screen size variables
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]){
    //Init SDL stuff
    SDL_Init(SDL_INIT_VIDEO);

    //SDL Window creation
    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    //SDL rendering init
    SDL_Surface *screen;
    screen = SDL_GetWindowSurface(window);

    //Main loop
    bool isRunning = true;
    while(isRunning){

        //Manage Events
        SDL_Event test_event;
        while(SDL_PollEvent(&test_event)){
            if(test_event.type == SDL_QUIT){
                isRunning = false;
            }
        }
        SDL_LockSurface(screen);
        //Clear Pixels
        int memSize = screen->h * screen->pitch;
        int size = screen->h * screen->w;
        SDL_memset(screen->pixels, 0, memSize );
        printf("Memset completed.\n");
        printf("Pixel Array size %d.\n", size);
        printf("Pixel memory size %d.\n", memSize);

        //Modify pixels
        Uint32 *pixels =(Uint32 *)screen->pixels;
        for(int i = 0; i < size; i++){
            if ((i % 10) == 0){
                //printf("Setting Value %d.\n", i);
                pixels[i] = 0xFF0000;
            }
        }

        SDL_UnlockSurface(screen);

        //Updating the screen   
        SDL_UpdateWindowSurface(window);
        SDL_Delay(1000);
    }
    
    //Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}