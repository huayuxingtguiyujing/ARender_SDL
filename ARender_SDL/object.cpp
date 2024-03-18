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
//// Sphere 球形
//SphereObj::SphereObj(Vec3f c, float r, MaterialType type) {
//	center = c;
//	radius = r;
//}
//
//SphereObj::~SphereObj() {
//}
//
//double SphereObj::intersection(const Vec3f& eyePos, const Vec3f& eyeDir) {
//	// 令球的方程为 (X - center)^2 = R^2;		
//	// 其中 X 为球面上的点
//	// 令射线的方程为 eyePos + t * eyeDir = L(t);
//	// 其中 t 为线段上的点
//	// 解一元二次方程
//
//	double a = eyeDir * eyeDir;	// 就是1
//	double b = eyeDir * (eyePos - center) * 2;
//	double c = (eyePos - center) * (eyePos - center) - radius * radius;
//	double deltaItem = b*b - 4*a*c;
//
//	//std::cout << deltaItem << std::endl;
//	if (deltaItem >= 0) {
//		// 有两个解 或 一个解，返回其中较小的那个
//		double ans1 = (-b + sqrt(deltaItem)) / (2 * a);
//		double ans2 = (-b - sqrt(deltaItem)) / (2 * a);
//		return ans1 > ans2 ? ans1 : ans2;
//	}
//	else {
//		// 没有解，直接返回 -1
//		// 返回值为负数时表示在射线的另一端相交，不会检测
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
//// MeshTriangle 三角形
//MeshTriangleObj::MeshTriangleObj(Vec3f a, Vec3f b, Vec3f c, MaterialType type = Diffuse_Glossy){
//	// 很奇怪... 初始化三角形必须按顺时针传入节点 顺序会影响渲染结果
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
//	// 使用 MT 算法
//	Vec3f E1 = b - a, E2 = c - a, S = eyePos - a;
//	Vec3f S1 = eyeDir^E2, S2 = S^E1;
//	if (S1 * E1 <= 0) {
//		return -1;
//	}
//	
//	// 求 交点 t 和 u v
//	float delta = S1 * E1;
//	float invdel = 1 / delta;
//	float tnear = S2 * E2 * invdel;
//	float u = S1 * S * invdel;
//	float v = S2 * eyeDir * invdel;
//
//	// 判断点是否在三角形内
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
