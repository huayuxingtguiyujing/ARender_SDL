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

	// ��Ⱦָ��ģ��
	void renderModel(Model* model, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, 
		float width, float height, const SDL_PixelFormat* format);

	// ������Ļ���ػ�����
	bool createBuffers(int w, int h);
	Buffer<Uint32>* getScreenBuffer();
	Buffer<float>* getZBuffer();
	void clearBuffers();

private:
	SceneHandler* sceneHandler;

	Buffer<float>* zBuffer;
	Buffer<Uint32>* pixelBuffer;

};

