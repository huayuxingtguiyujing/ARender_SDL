#include "object.h"

//Object::Object() {
//	Kd = 0.8;
//	Ks = 0.2;
//	diffuseColor = 0.2;
//	materialType = Diffuse_Glossy;
//}
//Object::Object(float kd, float ks, float dif, MaterialType type) {
//	Kd = kd;
//	Ks = ks;
//	diffuseColor = dif;
//	materialType = type;
//}
//Object::~Object() {
//}
//
//double Object::intersection(const Vec3f& eyePos, const Vec3f& eyeDir) {
//	return -1;
//}
//
//Vec3f Object::getNormal(const Vec3f& pos) {
//	return Vec3f(0,0,0);
//}
//
//// Sphere ����
//SphereObj::SphereObj(Vec3f c, float r, MaterialType type) {
//	center = c;
//	radius = r;
//}
//
//SphereObj::~SphereObj() {
//}
//
//double SphereObj::intersection(const Vec3f& eyePos, const Vec3f& eyeDir) {
//	// ����ķ���Ϊ (X - center)^2 = R^2;		
//	// ���� X Ϊ�����ϵĵ�
//	// �����ߵķ���Ϊ eyePos + t * eyeDir = L(t);
//	// ���� t Ϊ�߶��ϵĵ�
//	// ��һԪ���η���
//
//	double a = eyeDir * eyeDir;	// ����1
//	double b = eyeDir * (eyePos - center) * 2;
//	double c = (eyePos - center) * (eyePos - center) - radius * radius;
//	double deltaItem = b*b - 4*a*c;
//
//	//std::cout << deltaItem << std::endl;
//	if (deltaItem >= 0) {
//		// �������� �� һ���⣬�������н�С���Ǹ�
//		double ans1 = (-b + sqrt(deltaItem)) / (2 * a);
//		double ans2 = (-b - sqrt(deltaItem)) / (2 * a);
//		return ans1 > ans2 ? ans1 : ans2;
//	}
//	else {
//		// û�н⣬ֱ�ӷ��� -1
//		// ����ֵΪ����ʱ��ʾ�����ߵ���һ���ཻ��������
//		return -1;
//	}
//
//}
//
//Vec3f SphereObj::getNormal(const Vec3f& pos) {
//	Vec3f normal = pos - center;
//	return normal.normalize();
//}
//
//// MeshTriangle ������
//MeshTriangleObj::MeshTriangleObj(Vec3f a, Vec3f b, Vec3f c, MaterialType type = Diffuse_Glossy){
//	// �����... ��ʼ�������α��밴˳ʱ�봫��ڵ� ˳���Ӱ����Ⱦ���
//	this->a = a;
//	this->b = b;
//	this->c = c;
//
//	materialType = type;
//}
//
//MeshTriangleObj::~MeshTriangleObj() {
//}
//
//double MeshTriangleObj::intersection(const Vec3f& eyePos, const Vec3f& eyeDir) {
//
//	// ʹ�� MT �㷨
//	Vec3f E1 = b - a, E2 = c - a, S = eyePos - a;
//	Vec3f S1 = eyeDir^E2, S2 = S^E1;
//	if (S1 * E1 <= 0) {
//		return -1;
//	}
//	
//	// �� ���� t �� u v
//	float delta = S1 * E1;
//	float invdel = 1 / delta;
//	float tnear = S2 * E2 * invdel;
//	float u = S1 * S * invdel;
//	float v = S2 * eyeDir * invdel;
//
//	// �жϵ��Ƿ�����������
//	if (tnear >= 0 && u >= 0 && v >= 0 && (1 - u - v) >= 0) {
//		return tnear;
//	}
//	return -1;
//
//}
//
//Vec3f MeshTriangleObj::getNormal(const Vec3f& pos) {
//	Vec3f E1 = b - a;
//	Vec3f E2 = c - a;
//	Vec3f normal = E1 ^ E2;
//	return normal.normalize();
//}
