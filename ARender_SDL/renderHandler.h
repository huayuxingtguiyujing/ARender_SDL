#pragma once

#include "buffer.h"
#include "model.h"

#include "rasterization.h"
#include "sceneHandler.h"

#include <iostream>

class RenderHandler
{
public:
	RenderHandler();
	~RenderHandler();

	bool initRenderHanlder(int w, int h, SceneHandler* sceneHandler);
	void endRenderHandler();

	// 渲染指定模型
	void renderModel(Model* model, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, 
		float width, float height, const SDL_PixelFormat* format);

	// 管理屏幕像素缓冲区
	bool createBuffers(int w, int h);
	Buffer<Uint32>* getScreenBuffer();
	Buffer<float>* getZBuffer();
	void clearBuffers();

private:
	SceneHandler* sceneHandler;

	Buffer<float>* zBuffer;
	Buffer<Uint32>* pixelBuffer;

};

