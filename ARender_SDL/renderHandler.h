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

	// 渲染场景模型 -光栅化
	void renderModel(Model* model, float width, float height, const SDL_PixelFormat* format);
	void renderAllModel(float width, float height, const SDL_PixelFormat* format);

	// 渲染场景Object -光线追踪
	void renderObject(Object* object, float width, float height, const SDL_PixelFormat* format);
	void renderAllObj(float width, float height, const SDL_PixelFormat* format);
	bool isIntersect(const Light* light, double& intersection, int& objId, std::vector<Object*> recObj);
	bool tracing(const Light* lookDir, const Light* light, int reflectNum, float& diffuse, float& specular, std::vector<Object*> recObj);

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

