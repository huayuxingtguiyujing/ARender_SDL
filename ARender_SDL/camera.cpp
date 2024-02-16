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
}

Vec3f Camera::getCameraPos() {
	return position;
}
