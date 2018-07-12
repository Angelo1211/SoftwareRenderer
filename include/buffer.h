#ifndef BUFFER_H
#define BUFFER_H

#include "SDL.h"
#include <type_traits>

template<class T>
struct Buffer{
        int mWidth;
        int mHeight;
        int mPixelCount;
        int mPitch;
        T *buffer;

        T& operator()(size_t x, size_t y){
            return buffer[y*mWidth + x];
        }

        Buffer(int w, int h, T * array) 
        : mWidth(w), mHeight(h), mPixelCount(w*h),
                mPitch(w*sizeof(T)), buffer(array) 
        {}
        ~Buffer(){
                delete [] buffer;
        };
        
        void clear(){
                if(std::is_same<T,float>::value){
                        for(int i = 0; i < mPixelCount;++i){
                                buffer[i]  = 0.0f;
                        }
                }
                else{
                        memset(buffer,0, mPitch*mHeight);       
                }
        }
        
};

#endif