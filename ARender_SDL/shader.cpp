#include "shader.h"

IShader::IShader() {

}
IShader::~IShader() {

}

BlinnPhongShader::BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light) {
	this->model = model;
	// MVP V
	ModelTrans = mTrans;
	CameraTrans = cTrans;
	ProjectionTrans = proTrans;
	ViewTrans = vTrans;
	// 光线方向
	light_dir = light;
}

BlinnPhongShader::~BlinnPhongShader() {
}

void BlinnPhongShader::vertex(int index, Vec3f*& triangleV)  {
	std::vector<int> face = model->faceVertex(index);
	std::vector<int> faceNormal = model->faceNormal(index);

	for (int j = 0; j < 3; j++) {
		Vec3f v = model->vert(face[j]);
		// 获取每个顶点的法向量
		// NOTICE: 似乎目前法向量的方向是反的，所以要乘 -1
		vertex_normals[j] = model->normal(faceNormal[j]) * -1;
		// 计算世界坐标
		world_coords[j] = MatrixToVec(CameraTrans * ModelTrans * VecToMatrix(v));
		// 计算屏幕坐标
		screen_coords[j] = MatrixToVec(ViewTrans * VecToMatrix(world_coords[j]));
	}

	triangleV = screen_coords;
}

bool BlinnPhongShader::fragment(Vec3f bc_screen, Color& color)  {
	// 重心坐标插值求法向量
	Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];

	// 重心坐标插值求颜色
	// 光照部分仅计算了漫反射 Ld = kd * (I / r^2) * max(0, n * light_dir);
	float intensity = light_dir * vertexNormal;
	float r = intensity * 255;
	float g = intensity * 255;
	float b = intensity * 255;
	color = Color(r, g, b, 1);

	return true;
}