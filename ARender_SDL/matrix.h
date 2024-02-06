#pragma once


#include <fstream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SDL_stdinc.h>

#include "vector.h"

const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float> > m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	// ����Ļ�������
	static Matrix identity(int dimensions);

	std::vector<float>& operator[](const int i);

	Matrix operator*(const Matrix& a);

	Matrix transpose();

	Matrix inverse();


	// ģ�ͱ任
	static Matrix getZoom(float ratio);

	static Matrix getTranslate(float tx, float ty, float tz);

	static Matrix getRotationX(float anglex);

	static Matrix getRotationY(float angley);

	static Matrix getRotationZ(float anglez);

	// ������任
	static Matrix getCameraTrans();

	// ͶӰ�任
	static Matrix getPersProjection(float znear, float zfar, float aspectRatio, float eyeAngle);

	// ��ȡ�ӿڱ任����
	static Matrix getViewport(int x, int y, int w, int h);


	// ���� << �����
	friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

