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
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *gTexture = nullptr;
Uint32 *gBuffer = nullptr;
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
            //Create render instance
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == nullptr){
                printf("Could not create renderer context. Error: %s\n", SDL_GetError());
                success = false;
            }
            else{

                //Create Texture instance
                gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
                if(gTexture == nullptr){
                    printf("Could not create texture. Error: %s\n", SDL_GetError());
                    success = false;
                }
                else{
                    //Create pixel buffer
                    gBuffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
                    SDL_memset(gBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32) );
                }
            }
        }

    }

    return success;
}

void close(){
    delete[] gBuffer;
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    gBuffer = nullptr;
    gTexture = nullptr;
    gRenderer = nullptr;
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

void modifyPixels(){
    //Get window pixel format
    Uint32 format = SDL_PIXELFORMAT_RGBA8888;
    SDL_PixelFormat *mappingFormat = SDL_AllocFormat (format);

    //Get pixel count
    int pixelCount = SCREEN_HEIGHT * SCREEN_WIDTH;

    //Set color data
    Uint32 red1 = SDL_MapRGBA(mappingFormat, 0xFF,0x00,0x00,0x60);
    Uint32 green = SDL_MapRGBA(mappingFormat, 0x00,0xFF,0x00,0x80);
    Uint32 blue = SDL_MapRGBA(mappingFormat, 0x00,0x00,0xFF,0xFF);
    //Color in certain pixels
    for(int i = 0; i < pixelCount; ++i){
        if( (i % 50) == 0){
            gBuffer[i] = red1;
        }
        if((i % 1000) == 0){
            gBuffer[i] = green;
        }
        if((i % 2000) == 0){
            gBuffer[i] = blue;
        }
    }
}

// Uint32 getPixel(SDL_Surface &surface, int x, int y){
//     Uint32 *pixels = (Uint32 *)gScreen->pixels;
//     int arrayIndex = x + (gScreen->pitch * y);
//     return  pixels[arrayIndex];
// }

// void setPixel(SDL_Surface &surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
//     Uint32 *pixels = (Uint32 *)gScreen->pixels;
//     int arrayIndex = x + (gScreen->pitch * y);
//     pixels[arrayIndex] == r;
// }

void render(){

    //Clear Screen back to black
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    //Perform any modifications we want on the pixels
    modifyPixels();

    //Apply the pixel change to the texture
    SDL_UpdateTexture(gTexture, NULL, gBuffer, SCREEN_WIDTH*sizeof(Uint32) );
    
    //Render texture to screen
    SDL_SetTextureBlendMode(gTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(gRenderer, gTexture, NULL , NULL);

    //Update screen to window
    //SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderPresent( gRenderer );
}

void loop(){
    while(!quitFlag){

        //Manage Events
        eventManager();

        //Render pipeline
        render();

        SDL_Delay(500);
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
   //Free all memory and set all pointers to null
   close();

   return 0;
}