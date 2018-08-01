#ifndef BUFFER_H
#define BUFFER_H

#include "SDL.h"
#include <type_traits>

//Templated struct to emulate GPU buffers such as 
//the frame buffer and the ZBuffer along with others
//Also keeps track of a bunch of useful data about itself
template<class T>
struct Buffer{
        int mWidth;
        int mHeight;
        int mPixelCount;
        int mPitch;
        int mOrigin;
        T *buffer;

        T& operator()(size_t x, size_t y){
            return buffer[mOrigin + -y*mWidth + x];
        }

        Buffer(int w, int h, T * array) 
        : mWidth(w), mHeight(h), mPixelCount(w*h),
                mPitch(w*sizeof(T)),mOrigin(mHeight*mWidth - mWidth), buffer(array) 
        {}

        ~Buffer(){
                delete [] buffer;
        };
        
        //Cannot use memset to clear a float since the binary
        //Representation is more complex. We just iterate through the whole
        //thing and explicitely set it to zero instead
        void clear(){
                if(std::is_same<T,float>::value){
                        for(int i = 0; i < mPixelCount;++i){
                                buffer[i]  = 0.0f;
                        }
                }
                else{
                        memset(buffer,0xD, mPitch*mHeight);       
                }
        }
};

#endif