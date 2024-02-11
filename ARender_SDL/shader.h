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

	// ���շ���
	Vec3f light_dir{ 0, 0, -1 };

	// MVP �任����
	Matrix ModelTrans, CameraTrans, ProjectionTrans;

	// ��ͼ�任����
	Matrix ViewTrans;

	// ��ǰ���ڱ����� ����/ƬԪ ����
	Vec3f vertex_normals[3], screen_coords[3], world_coords[3];

	BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);

	~BlinnPhongShader();

	void vertex(int index, Vec3f*& triangleV) override;

	bool fragment(Vec3f bc_screen, Color& color) override;

};
