#ifndef CANVAS_H
#define CANVAS_H

    #include "SDL.h"

    struct Canvas{
            Canvas(int w, int h, int px, int pi, Uint32 * buf) 
                : mWidth(w), mHeight(h), mPixelCount(px), mPitch(pi),mBuffer(buf) 
            {
            }
            int mWidth;
            int mHeight;
            int mPixelCount;
            int mPitch;
            Uint32 *mBuffer; 
    };

#endif