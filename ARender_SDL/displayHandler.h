#pragma once
#include <SDL.h>

#include "buffer.h"

class DisplayHandler
{
public:
	// SDL 窗口的宽高
	const static int screenWidth = 800;
	const static int screenHeight = 600;
	float static getScreenRatio() {
		return screenWidth / (float)screenHeight;
	}

	DisplayHandler();
	~DisplayHandler();

	bool initDisplayHandler();
	void endDisplayHandler();

	// 更新视图
	void updateDisplay();

	// 采用双缓冲分配器，刷新每帧
	void swapBuffers(Buffer<Uint32>* pixelBuffer);

	SDL_PixelFormat* getSDLFormat();

private:

	SDL_Surface* surface;
	SDL_Window* window;
};

