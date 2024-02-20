#pragma once

#include <vector>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <SDL_stdinc.h>

template <class t> struct Vec2 {
	union {
		struct { t u, v; };
		struct { t x, y; };
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u), v(_v) {}
	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u * f, v * f); }
	t& operator[](const size_t i) { return i <= 0 ? x : y; }

	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	union {
		struct { t x, y, z; };
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}

	// 矢量基本运算
	inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
	inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
	t& operator[](const size_t i) {
		if (i == 2) return z;
		else if (i == 1) return y;
		else return x;
	}

	// 规范化矢量
	float norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }

	// 用于 blinn shading 计算高光项
	Vec3 reflect(const Vec3& N) {
		return *this - (N * (*this * N) * 2.0f);
	}

	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

// 二维矢量 与 三维矢量
typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

// 矩阵的默认维度
const int DEFAULT_ALLOC = 4;

class Matrix {
	std::vector<std::vector<float> > m;
	int rows, cols;
public:
	Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
	inline int nrows();
	inline int ncols();

	// 矩阵的基本操作
	static Matrix identity(int dimensions);

	std::vector<float>& operator[](const int i);

	Matrix operator*(const Matrix& a);

	Matrix transpose();

	Matrix inverse();


	// 模型变换
	static Matrix getZoom(float ratio);

	static Matrix getTranslate(float tx, float ty, float tz);

	static Matrix getRotationX(float anglex);

	static Matrix getRotationY(float angley);

	static Matrix getRotationZ(float anglez);

	// 摄像机变换
	static Matrix getCameraTrans();

	// 投影变换
	static Matrix getPersProjection(float znear, float zfar, float aspectRatio, float eyeAngle);

	// 获取视口变换矩阵
	static Matrix getViewport(int x, int y, int w, int h);


	// 重载 << 运算符
	friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

Matrix VecToMatrix(Vec3f vec);

Vec3f MatrixToVec(Matrix m);
