#pragma once

#include "geometry.h"
#include "displayHandler.h"


// һ��ƽ�� ��������
// Ax + By + Cz + D = 0;
class Plane {
	Vec3f normal;
	float D;		// Dֵ

public:
	// ʹ�õ�ͷ���������ƽ��
	Plane();
	Plane(Vec3f normal, Vec3f point);

	void setPlane(Vec3f normal, Vec3f point);

	float distance(Vec3f point);
};

// ������۲�����
class Frustrum {
private:
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};
public:
	// AR: aspect ratio
	float  fov, AR, near, far, nearH, nearW;
	Plane planes[6];

	Frustrum();
	Frustrum(float ratio);

	void updateFrustrum(Matrix& viewMat, const Vec3f& cameraPos);

};



class Camera
{
public:
	
	Matrix viewMat;
	Matrix projectionMat;

	// �����������׶
	Frustrum cameraFrustrum;

	Camera() : up(Vec3f(0, 1, 0)), gazeDir(Vec3f(0, 0, -1)), right(Vec3f(1, 0, 0)), position(Vec3f(0,0,-5)){
		viewMat = getCameraTrans();
		
		cameraFrustrum = Frustrum(DisplayHandler::getScreenRatio());
		projectionMat = Matrix::getPersProjection2(cameraFrustrum.fov, cameraFrustrum.AR, cameraFrustrum.near, cameraFrustrum.far);
		cameraFrustrum.updateFrustrum(viewMat, position);
	}
	Camera(Vec3f up, Vec3f gaze, Vec3f pos) {
		this->position = pos;
		this->up = up;
		this->gazeDir = gaze;

		right = gazeDir ^ up;
	}
	
	Matrix getCameraTrans();

	void MoveCamera(float tx, float ty, float tz);
	void RotateCamera(float rx, float ry, float rz);

	Vec3f getCameraPos();

private:
	// up, right, front(gaze dir)
	Vec3f up;
	Vec3f right;
	Vec3f gazeDir;

	Vec3f position;

};
