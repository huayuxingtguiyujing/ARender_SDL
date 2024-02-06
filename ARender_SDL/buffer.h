#pragma once

#include "SDL.h"
#include <type_traits>
#include <string.h> 

template<class T>
struct Buffer
{
    int mWidth, mHeight, mPixelCount, mPitch, mOrigin;
    T* buffer;


    T& operator()(size_t x, size_t y) {
        return buffer[mOrigin - y * mWidth + x];
    }

    Buffer(int w, int h, T* array)
        : mWidth(w), mHeight(h), mPixelCount(w* h), mPitch(w * sizeof(T)),
        mOrigin(mHeight* mWidth - mWidth), buffer(array) {
        // mOrigin 从存储区的左下角开始
    }

    ~Buffer() { delete[] buffer; }


    void clear() {
        if (std::is_same<T, float>::value) {
            for (int i = 0; i < mPixelCount; i++) {
                buffer[i] = 0.0f;
            }
        }
        else {
            //Set to a 15% white color to make it nicer looking.
            memset(buffer, 0xD, mPitch * mHeight);
        }
    }
};

