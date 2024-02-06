#pragma once

#include "matrix.h"
#include "vector.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <SDL.h>

#include "buffer.h"
#include "color.h"
#include "math.h"

class Rasterization
{
public:

	// ����
	static void line(int x0, int y0, int x1, int y1);

	static void line(Vec3f p0, Vec3f p1);

	// trans vector to matrix
	static Matrix VecToMatrix(Vec3f vec);

	static Vec3f MatrixToVec(Matrix m);

	// �жϵ��Ƿ�����������
	static bool insideTriangle(float x, float y, const std::vector<Vec3f> triangleV);

	// ��ȡ�����ε��������� ����(u,v,w)
	static Vec3f getBarycentric(Vec3f A, Vec3f B, Vec3f C);

	static Vec3f getBarycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

	// ��������
	static void triangle(Vec3f* triangleV, float* zbuffer, Color color, float width, float height);

	static void triangle(Vec3f* triangleV, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, Color color, float width, float height, const SDL_PixelFormat* MapFormat);

private:
	static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const SDL_PixelFormat* mappingFormat;

};

