#pragma once

#include "geometry.h"

// NOTICE: 3.18�գ����ֺϲ���ͻ���⣬�ָ��ļ��µ����Ѿ�ת����math�ļ���
//enum MaterialType {
//	Reflection,
//	Diffuse_Glossy,
//	Reflection_Refraction
//};
//
//// ��ʾ�����еļ����壬������Ϊ����������Ļ���
//class Object
//{
//public:
//	MaterialType materialType;
//
//	// ���� ϵ��
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
//// Sphere ����
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
//// MeshTriangle ������
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

// �ǲ���Ӧ����д�� ������Χ�� (AABB)
// �ռ仮�� �� ���ٽṹ
