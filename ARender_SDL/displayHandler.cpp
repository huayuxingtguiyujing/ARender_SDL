#include "displayHandler.h"

#include <iostream>

DisplayHandler::DisplayHandler() {

}

DisplayHandler::~DisplayHandler() {

}

bool DisplayHandler::initDisplayHandler() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "初始化 SDL 失败: " << SDL_GetError() << std::endl;
        return  false;
    }

    // 创建窗口
    window = SDL_CreateWindow("ARender_SDL",SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);

    if (window == nullptr) {
        std::cout << "创建 SDL 窗口失败: " << SDL_GetError() << std::endl;
        return false;
    }

    // 创建表面（自己看api吧）
    surface = SDL_GetWindowSurface(window);

    if (surface == nullptr) {
        std::cout << "创建 SDL 表面失败: " << SDL_GetError() << std::endl;
        return false;
    }


	return true;
}

void DisplayHandler::endDisplayHandler() {
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

void DisplayHandler::updateDisplay() {

}

// 双重缓冲, 进行绘制
void DisplayHandler::swapBuffers(Buffer<Uint32>* pixelBuffer) {
    // 锁住surface，以开始读写
    SDL_LockSurface(surface);

    // 从缓存中copy到surface上
    memcpy(surface->pixels, pixelBuffer->buffer, pixelBuffer->mHeight * pixelBuffer->mPitch);
    SDL_UnlockSurface(surface);

    // 更新
    SDL_UpdateWindowSurface(window);
}

SDL_PixelFormat* DisplayHandler::getSDLFormat() {
    return surface->format;
}