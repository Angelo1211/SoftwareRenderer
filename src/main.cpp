#include <stdio.h>
#include "SDL.h"

//Screen size variables
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//SDL startup and window initialization
bool init();

//Cleanup of memory and shutdown SDL
void close();

//Keyboard and mouse input managing
void eventManager();

//Clear screen to 0 black pixels
void clearScreen();

//Simple pixel modification function
void modifyPixels();

//Render pipeline function
void render();

//Current main project loop
void loop();

//Global variables
SDL_Window  *gWindow = nullptr;
SDL_Surface *gScreen = nullptr;
bool quitFlag = false;

bool init(){
    //Initialization flag
    bool success = true;

    if( SDL_Init(SDL_INIT_VIDEO) != 0){
       printf("Failed to initialize SDL. Error: %s\n", SDL_GetError() );
       success = false;
    }
    else{
        //Window creation
        gWindow = SDL_CreateWindow( "SDL2Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if( gWindow == nullptr){
            printf("Coult not create window. Error: %s\n", SDL_GetError() );
            success = false;
        }
        else{
            gScreen = SDL_GetWindowSurface(gWindow);
        }

    }

    return success;
}

void close(){
    SDL_FreeSurface(gScreen);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    gScreen = nullptr;
    gWindow = nullptr;
}

void eventManager(){
    SDL_Event test_event;
    while(SDL_PollEvent(&test_event)){
        if(test_event.type == SDL_QUIT){
            quitFlag = true;
        }
    }
}

void clearScreen(){
    int memSize = gScreen->h * gScreen->pitch;
    SDL_memset(gScreen->pixels, 0, memSize );
}

void modifyPixels(){
    Uint32 *pixels =(Uint32 *)gScreen->pixels;
    int size = gScreen->h * gScreen->w;
        for(int i = 0; i < size; i++){
            if ((i % 10) == 0){
                pixels[i] = 0xFF0000;
            }
            if ((i % 20) == 0){
                pixels[i] = 0x00FF00;
            }
        }
}

void render(){
    //Open surface for pixel editing
    SDL_LockSurface(gScreen);

    //Clear Screen back to black
    clearScreen();
    
    //Perform modification function
    modifyPixels();

    //Close surface for pixel editing
    SDL_UnlockSurface(gScreen);
    
    //Update screen to window
    SDL_UpdateWindowSurface(gWindow);
}

void loop(){
    while(!quitFlag){

        //Manage Events
        eventManager();

        //Render pipeline
        render();

        //Wait in between frames
        SDL_Delay(16);
    }
}

int main(int argc, char *argv[]){
    //Init SDL stuff
   if (!init()){
       printf("Failed to initialize.\n");
   }
   else{
       //Main loop
        loop();
   }
   close();

   return 0;
}