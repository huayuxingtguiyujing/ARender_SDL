#pragma once

#include "geometry.h"

class Camera
{
public:
	
	Camera() : up(Vec3f(0, 1, 0)), gazeDir(Vec3f(0, 0, -1)), right(Vec3f(1, 0, 0)), position(Vec3f(0,0,0)){

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

