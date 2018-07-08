#ifndef CANVAS_H
#define CANVAS_H

    #include "SDL.h"

    struct Canvas{
            Canvas(int w, int h, int px, int pi, Uint32 * buf1, float * buf2) 
                : mWidth(w), mHeight(h), mPixelCount(px),
                 mPitch(pi),mBuffer(buf1), mDBuffer(buf2) 
            {}
            int mWidth;
            int mHeight;
            int mPixelCount;
            int mPitch;
            //Represents the pixel data buffer
            Uint32 *mBuffer; //FIX THIS AT SOME POINT  

            //Represents the depth data buffer
            float *mDBuffer;
    };

#endif