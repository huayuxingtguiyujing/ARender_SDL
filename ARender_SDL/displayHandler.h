#pragma once
#include <SDL.h>

#include "buffer.h"

class DisplayHandler
{
public:
	// SDL ���ڵĿ��
	const static int screenWidth = 800;
	const static int screenHeight = 600;
	float static getScreenRatio() {
		return screenWidth / (float)screenHeight;
	}

	DisplayHandler();
	~DisplayHandler();

	bool initDisplayHandler();
	void endDisplayHandler();

	// ������ͼ
	void updateDisplay();

	// ����˫�����������ˢ��ÿ֡
	void swapBuffers(Buffer<Uint32>* pixelBuffer);

	SDL_PixelFormat* getSDLFormat();

private:

	SDL_Surface* surface;
	SDL_Window* window;
};

