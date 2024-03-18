#pragma once

#include "geometry.h"

// NOTICE: 3.18日，出现合并冲突问题，现该文件下的类已经转到了math文件里
//enum MaterialType {
//	Reflection,
//	Diffuse_Glossy,
//	Reflection_Refraction
//};
//
//// 表示场景中的简单物体，用于作为其他简单物体的基类
//class Object
//{
//public:
//	MaterialType materialType;
//
//	// 光照 系数
//	float Kd;
//	float Ks;
//	float diffuseColor;
//
//	Object();
//	Object(float kd, float ks, float dif, MaterialType type);
//	~Object();
//
//	double virtual intersection(const Vec3f& eyePos, const  Vec3f& eyeDir);
//	Vec3f virtual getNormal(const Vec3f& pos);
//};
//
//// Sphere 球形
//class SphereObj : public Object {
//public:
//	Vec3f center;
//	float radius;
//	
//	SphereObj(Vec3f c, float r, MaterialType type);
//	~SphereObj();
//
//	double intersection(const Vec3f& eyePos, const  Vec3f& eyeDir) override;
//	Vec3f getNormal(const Vec3f& pos) override;
//};
//
//// MeshTriangle 三角形
//class MeshTriangleObj : public Object {
//public:
//	Vec3f a;
//	Vec3f b;
//	Vec3f c;
//
//	MeshTriangleObj(Vec3f a, Vec3f b, Vec3f c, MaterialType type);
//	~MeshTriangleObj();
//
//	double intersection(const Vec3f& eyePos, const  Vec3f& eyeDir) override;
//	Vec3f getNormal(const Vec3f& pos) override;
//};

// 是不是应该再写个 轴对齐包围盒 (AABB)
// 空间划分 和 加速结构
