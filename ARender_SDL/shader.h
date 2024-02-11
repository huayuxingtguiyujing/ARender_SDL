#pragma once

#include "model.h"
#include "math.h"
#include "color.h"

#include "geometry.h"

struct IShader {

	IShader();
	~IShader();

	virtual void vertex(int index, Vec3f* &triangleV) = 0;
	virtual bool fragment(Vec3f bc_screen, Color& color) = 0;
};

struct BlinnPhongShader : IShader {

	Model* model;

	// 光照方向
	Vec3f light_dir{ 0, 0, -1 };

	// MVP 变换矩阵
	Matrix ModelTrans, CameraTrans, ProjectionTrans;

	// 视图变换矩阵
	Matrix ViewTrans;

	// 当前正在遍历的 顶点/片元 数据
	Vec3f vertex_normals[3], screen_coords[3], world_coords[3];

	BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);

	~BlinnPhongShader();

	void vertex(int index, Vec3f*& triangleV) override;

	bool fragment(Vec3f bc_screen, Color& color) override;

};
