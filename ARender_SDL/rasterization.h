#pragma once


#include "geometry.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <SDL.h>

#include "buffer.h"
#include "color.h"
#include "shader.h"

class Rasterization
{
public:

	// ����
	static void line(int x0, int y0, int x1, int y1);

	static void line(Vec3f p0, Vec3f p1);

	// �жϵ��Ƿ�����������
	static bool insideTriangle(float x, float y, const std::vector<Vec3f> triangleV);

	// ��ȡ�����ε��������� ����(u,v,w)
	static Vec3f getBarycentric(Vec3f A, Vec3f B, Vec3f C);

	static Vec3f getBarycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

	// ��������
	static void triangle(Vec3f* triangleV, float* zbuffer, Color color, float width, float height);

	static void triangle(Vec3f* world_triangleV, Vec3f* triangleV, Vec3f* vertex_normals, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, 
		Buffer<float>* zBuffer_SSAA, Buffer<Uint32>* pixelBuffer_SSAA,
		Vec3f lightDir, float width, float height, const SDL_PixelFormat* MapFormat);

	static void triangle(Vec3f* triangleV, IShader& shader, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, const SDL_PixelFormat* MapFormat);

	static void triangle(Vec3f* triangleV, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, Color color, const SDL_PixelFormat* MapFormat);

private:
	static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const SDL_PixelFormat* mappingFormat;

};

