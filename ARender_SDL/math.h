#pragma once

#include "geometry.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cstdlib>



enum MaterialType {
	Reflection,
	Diffuse_Glossy,
	Reflection_Refraction
};

// 表示场景中的简单物体，用于作为其他简单物体的基类
class Object
{
public:
	MaterialType materialType;

	// 光照 系数
	float Kd;
	float Ks;
	float diffuseColor;

	Object();
	Object(float kd, float ks, float dif, MaterialType type);
	~Object();

	double virtual intersection(const Vec3f& eyePos, const  Vec3f& eyeDir);
	Vec3f virtual getNormal(const Vec3f& pos);
};

// Sphere 球形
class SphereObj : public Object {
public:
	Vec3f center;
	float radius;

	SphereObj(Vec3f c, float r, MaterialType type);
	~SphereObj();

	double intersection(const Vec3f& eyePos, const  Vec3f& eyeDir) override;
	Vec3f getNormal(const Vec3f& pos) override;
};

// MeshTriangle 三角形
class MeshTriangleObj : public Object {
public:
	Vec3f a;
	Vec3f b;
	Vec3f c;

	MeshTriangleObj(Vec3f a, Vec3f b, Vec3f c, MaterialType type);
	~MeshTriangleObj();

	double intersection(const Vec3f& eyePos, const  Vec3f& eyeDir) override;
	Vec3f getNormal(const Vec3f& pos) override;
};

// 是不是应该再写个 轴对齐包围盒 (AABB)
// 空间划分 和 加速结构

struct BoundingBox {

	float minX;
	float minY;
	float maxX;
	float maxY;

	BoundingBox(float minX, float minY, float maxX, float maxY) {
		this->minX = minX;
		this->minY = minY;
		this->maxX = maxX;
		this->maxY = maxY;
	}

	//BoundingBox(Vec3f * triangleV) {
	//	std::vector<Vec3f> rec = std::vector<Vec3f> {triangleV[0], triangleV[1], triangleV[2] };
	//	BoundingBox(rec);
	//}

	BoundingBox(Vec3f* triangleV, int width, int height) {
		minX = std::numeric_limits<float>::max();
		minY = std::numeric_limits<float>::max();
		maxX = -std::numeric_limits<float>::max();
		maxY = -std::numeric_limits<float>::max();
		for (int i = 0; i < 3; i++) {
			minX = std::min(triangleV[i].x, minX);
			minY = std::min(triangleV[i].y, minY);
			maxX = std::max(triangleV[i].x, maxX);
			maxY = std::max(triangleV[i].y, maxY);
		}

		// 不能超过的屏幕的宽高
		if (minX < 0) minX = 0.f;
		if (minY < 0) minY = 0.f;
		if (maxX >= width) maxX = width - 1;
		if (maxY >= height) maxY = height - 1;

	}

};