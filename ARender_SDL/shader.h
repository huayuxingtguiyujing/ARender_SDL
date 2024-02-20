#pragma once

#include "model.h"
#include "math.h"
#include "color.h"

#include "geometry.h"

struct IShader {

	IShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~IShader();

	Model* model;

	// 光照方向
	Vec3f light_dir{ 0, 0, -1 };

	// MVP 变换矩阵
	Matrix ModelTrans, CameraTrans, ProjectionTrans;

	// 视图变换矩阵
	Matrix ViewTrans;

	// 当前正在遍历的 顶点/片元 数据
	Vec3f vertex_normals[3], screen_coords[3], world_coords[3], uv_coords[3];
	// 当前遍历 三个顶点 颜色
	Vec3f uv_color[3];		


	virtual void vertex(int index, Vec3f* &triangleV) = 0;
	virtual bool fragment(Vec3f bc_screen, Color& color) = 0;
};

// Flat Shader
// 通过三角形三顶点坐标 计算出三角形的法向量
// Triangle face is flat — one normal vector
struct FlatShader : IShader {

	FlatShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~FlatShader();
	
	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};

// Gouraud Shader
// 通过片元顶点 插值计算每个点的法向量
// 在顶点阶段完成着色计算 （Phong Shader是在片元阶段）
struct GouraudShader : IShader {

	GouraudShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~GouraudShader();

	float diffuse[3], specular[3];

	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};

// PhongShader
// Interpolate normal vectors across each triangle
// 在片元阶段完成着色计算

// BlinnPhongShader
// 
struct BlinnPhongShader : IShader {

	BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light);
	~BlinnPhongShader();

	void vertex(int index, Vec3f*& triangleV) override;
	bool fragment(Vec3f bc_screen, Color& color) override;
};
