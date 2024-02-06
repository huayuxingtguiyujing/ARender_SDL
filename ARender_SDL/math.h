#pragma once

#include "vector.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cstdlib>


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