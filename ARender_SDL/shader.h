#pragma once

#include "model.h"
#include "math.h"
#include "color.h"

#include "geometry.h"

struct IShader {

	IShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~IShader();

	Model* model;

	// ���շ���
	Vec3f light_dir{ 0, 0, -1 };

	// MVP �任����
	Matrix ModelTrans, CameraTrans, ProjectionTrans;

	// ��ͼ�任����
	Matrix ViewTrans;

	// ��ǰ���ڱ����� ����/ƬԪ ����
	Vec3f vertex_normals[3], screen_coords[3], world_coords[3], uv_coords[3];
	// ��ǰ���� �������� ��ɫ
	Vec3f uv_color[3];		


	virtual void vertex(int index, Vec3f* &triangleV) = 0;
	virtual bool fragment(Vec3f bc_screen, Color& color) = 0;
};

// Flat Shader
// ͨ������������������ ����������εķ�����
// Triangle face is flat �� one normal vector
struct FlatShader : IShader {

	FlatShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~FlatShader();
	
	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};

// Gouraud Shader
// ͨ��ƬԪ���� ��ֵ����ÿ����ķ�����
// �ڶ���׶������ɫ���� ��Phong Shader����ƬԪ�׶Σ�
struct GouraudShader : IShader {

	GouraudShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~GouraudShader();

	float diffuse[3], specular[3];

	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};

// PhongShader
// Interpolate normal vectors across each triangle
// ��ƬԪ�׶������ɫ����

// BlinnPhongShader
// 
struct BlinnPhongShader : IShader {

	BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~BlinnPhongShader();

	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};
