#pragma once

#include "buffer.h"
#include "model.h"

#include "rasterization.h"
#include "sceneHandler.h"
#include "shader.h"

#include <iostream>

class RenderHandler
{
public:

	const static int SSAA_W = 2;
	const static int SSAA_H = 2;

	RenderHandler();
	~RenderHandler();

	bool initRenderHanlder(int w, int h, SceneHandler* sceneHandler);
	void endRenderHandler();

	// 渲染指定模型
	void renderModel(Model* model, float width, float height, const SDL_PixelFormat* format);
	void renderModel2(float width, float height, const SDL_PixelFormat* format);
	void renderAllModel(float width, float height, const SDL_PixelFormat* format);

	// 管理屏幕像素缓冲区
	bool createBuffers(int w, int h);
	void clearBuffers();
	
	Buffer<float>* getZBuffer();
	Buffer<float>* getZBuffer_SSAA();
	Buffer<Uint32>* getScreenBuffer();
	Buffer<Uint32>* getScreenBuffer_SSAA();

private:
	SceneHandler* sceneHandler;

	Buffer<float>* zBuffer;
	Buffer<float>* SSAA_zBuffer;

	Buffer<Uint32>* pixelBuffer;
	Buffer<Uint32>* SSAA_pixelBuffer;

};

