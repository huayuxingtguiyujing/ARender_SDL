#include "displayHandler.h"

#include <iostream>

DisplayHandler::DisplayHandler() {

}

DisplayHandler::~DisplayHandler() {

}

bool DisplayHandler::initDisplayHandler() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "��ʼ�� SDL ʧ��: " << SDL_GetError() << std::endl;
        return  false;
    }

    // ��������
    window = SDL_CreateWindow("ARender_SDL",SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);

    if (window == nullptr) {
        std::cout << "���� SDL ����ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    // �������棨�Լ���api�ɣ�
    surface = SDL_GetWindowSurface(window);

    if (surface == nullptr) {
        std::cout << "���� SDL ����ʧ��: " << SDL_GetError() << std::endl;
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

// ˫�ػ���, ���л���
void DisplayHandler::swapBuffers(Buffer<Uint32>* pixelBuffer) {
    // ��סsurface���Կ�ʼ��д
    SDL_LockSurface(surface);

    // �ӻ�����copy��surface��
    memcpy(surface->pixels, pixelBuffer->buffer, pixelBuffer->mHeight * pixelBuffer->mPitch);
    SDL_UnlockSurface(surface);

    // ����
    SDL_UpdateWindowSurface(window);
}

SDL_PixelFormat* DisplayHandler::getSDLFormat() {
    return surface->format;
}