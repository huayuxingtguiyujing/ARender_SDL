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

// ��ʾ�����еļ����壬������Ϊ����������Ļ���
class Object
{
public:
	MaterialType materialType;

	// ���� ϵ��
	float Kd;
	float Ks;
	float diffuseColor;

	Object();
	Object(float kd, float ks, float dif, MaterialType type);
	~Object();

	double virtual intersection(const Vec3f& eyePos, const  Vec3f& eyeDir);
	Vec3f virtual getNormal(const Vec3f& pos);
};

// Sphere ����
class SphereObj : public Object {
public:
	Vec3f center;
	float radius;

	SphereObj(Vec3f c, float r, MaterialType type);
	~SphereObj();

	double intersection(const Vec3f& eyePos, const  Vec3f& eyeDir) override;
	Vec3f getNormal(const Vec3f& pos) override;
};

// MeshTriangle ������
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

// �ǲ���Ӧ����д�� ������Χ�� (AABB)
// �ռ仮�� �� ���ٽṹ

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

		// ���ܳ�������Ļ�Ŀ��
		if (minX < 0) minX = 0.f;
		if (minY < 0) minY = 0.f;
		if (maxX >= width) maxX = width - 1;
		if (maxY >= height) maxY = height - 1;

	}

};