#ifndef SRENDERER_H
#define SRENDERER_H

#include "displayManager.h"
#include "rasterizer.h" 
#include "buffer.h"

class SoftwareRenderer {

    public:
        //Dummy Constructor / Destructor
        SoftwareRenderer();
        ~SoftwareRenderer();

        //Creates all buffers and initializes all sub-programs needed
        //For rendering
        bool startUp(DisplayManager &displayManager);  
        void shutDown();      

        //void render(Model *models, Matrix4 &mat);
        void render();
        
    private:
        //Buffer methods
        bool createBuffers();
        void clearBuffers();

        DisplayManager * screen;
        Buffer<float> * zBuffer;
        Buffer<Uint32> * pixelBuffer;
        
        Rasterizer *raster;

        //Culling
        //bool frustrumCulling(Model *model, Matrix4 &viewMatrix);
};

#endif