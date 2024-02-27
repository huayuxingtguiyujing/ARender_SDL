#include "camera.h"

#include "rasterization.h"

Matrix Camera::getCameraTrans() {
	
	Matrix translate = Matrix::getTranslate(position.x, position.y, position.z);
	
	Vec3f w = Vec3f(0,0,0) - gazeDir.normalize();
	Vec3f u = (up ^ w).normalize();
	Vec3f v = w ^ u;
	
	Matrix rotation = Matrix::identity(4);
	rotation[0][0] = u.x;
	rotation[0][1] = u.y;
	rotation[0][2] = u.z;
	
	rotation[1][0] = v.x;
	rotation[1][1] = v.y;
	rotation[1][2] = v.z;

	rotation[2][0] = w.x;
	rotation[2][1] = w.y;
	rotation[2][2] = w.z;

	return rotation * translate;
}

void Camera::MoveCamera(float tx, float ty, float tz) {
	position = position + Vec3f(tx, ty, tz);
	std::cout << "move the camera: " << tx << "," << ty << "," << tz << std::endl;

	// 更新摄像机的位置 角度
	viewMat = getCameraTrans();
}

void Camera::RotateCamera(float rx, float ry, float rz) {
	Matrix rotationX = Matrix::getRotationX(rx);
	Matrix rotationY = Matrix::getRotationY(ry);
	Matrix rotationZ = Matrix::getRotationZ(rz);
	Matrix rotation = rotationX * rotationY * rotationZ;

	up = MatrixToVec(rotation * VecToMatrix(up));
	right = MatrixToVec(rotation * VecToMatrix(right));
	gazeDir = MatrixToVec(rotation * VecToMatrix(gazeDir));

	std::cout << "rotate the camera: " << rx << "," << ry << "," << rz << std::endl;

	// 更新摄像机的位置 角度
	viewMat = getCameraTrans();
}

Vec3f Camera::getCameraPos() {
	return position;
}

// 摄像机观察区域
Frustrum::Frustrum() {

}

Frustrum::Frustrum(float ratio){
	fov = 50;
	near = 0.1f;
	far = 100;
	AR = ratio;
}

void Frustrum::updateFrustrum(Matrix& viewMat, const Vec3f& cameraPos) {
	// 更新摄像机 视锥范围 各个 要素
	float tanHalfFOV = tan((fov / 2) * (M_PI / 180));
	nearH = near * tanHalfFOV;
	nearW = nearH * AR;
	
	// 根据 摄像机视角，摄像机位置 更新之
	Vec3f X(viewMat[0][0], viewMat[0][1], viewMat[0][2]); //Side Unit Vector
	Vec3f Y(viewMat[1][0], viewMat[1][1], viewMat[1][2]); //Up Unit Vector
	Vec3f Z(viewMat[2][0], viewMat[2][1], viewMat[2][2]); //Forward vector

	Vec3f nearCenter = cameraPos - Z * near;
	Vec3f farCenter = cameraPos - Z * far;

	Vec3f point;
	Vec3f normal;

	// 设置各个平面
	planes[NEARP].setPlane(Vec3f(0,0,0) - Z, nearCenter);

	//Far plane 
	planes[FARP].setPlane(Z, farCenter);

	point = nearCenter + Y * nearH;
	normal = (point - cameraPos).normalize();
	normal = normal ^ (X);
	planes[TOP].setPlane(normal, point);

	//Bottom plane
	point = nearCenter - Y * nearH;
	normal = (point - cameraPos).normalize();
	normal = X ^ (normal);
	planes[BOTTOM].setPlane(normal, point);

	//Left plane
	point = nearCenter - X * nearW;
	normal = (point - cameraPos).normalize();
	normal = normal ^ (Y);
	planes[LEFT].setPlane(normal, point);

	//Right plane
	point = nearCenter + X * nearW;
	normal = (point - cameraPos).normalize();
	normal = Y ^ (normal);
	planes[RIGHT].setPlane(normal, point);
}

// 一个平面 数据类型
Plane::Plane() {

}

Plane::Plane(Vec3f n, Vec3f point) {
	setPlane(n, point);
}

void Plane::setPlane(Vec3f n , Vec3f point) {
	normal = n;
	D = -(n * point);
}

float Plane::distance(Vec3f point) {
	return normal * point + D;
}
