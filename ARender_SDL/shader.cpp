#include "shader.h"

IShader::IShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light) {
	this->model = model;
	// MVP V
	ModelTrans = mTrans;
	CameraTrans = cTrans;
	ProjectionTrans = proTrans;
	ViewTrans = vTrans;
	// ���߷���
	light_dir = light;
}
IShader::~IShader() {

}

// Flat Shader
// 
FlatShader::FlatShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light)
	:IShader(model, mTrans, cTrans, proTrans, vTrans, light){
}
FlatShader::~FlatShader() {
}

void FlatShader::vertex(int index, Vec3f*& triangleV) {
	std::vector<int> face = model->faceVertex(index);
	std::vector<int> faceNormal = model->faceNormal(index);

	for (int j = 0; j < 3; j++) {
		Vec3f v = model->vert(face[j]);
		// ���㶥�㷨����
		vertex_normals[j] = model->normal(faceNormal[j]) * -1;
		// ������������
		world_coords[j] = MatrixToVec(CameraTrans * ModelTrans * VecToMatrix(v));
		// ������Ļ����
		screen_coords[j] = MatrixToVec(ViewTrans * VecToMatrix(world_coords[j]));
	}
	triangleV = screen_coords;
}

bool FlatShader::fragment(Vec3f bc_screen, Color& color) {
	Vec3f vertexNormal = vertex_normals[0] * 0.33f + vertex_normals[1] * 0.33f + vertex_normals[2] * 0.33f;
	float intensity = light_dir * vertexNormal;
	// flat shader �����ȡ�������������
	color = Color(intensity * 255, intensity * 255, intensity * 255, 1);
	// ERROR: Ŀǰʹ�� flat shader ���ɵ�ͼ��ƬԪ���������
	return true;
}

// GouraudShader
// 
GouraudShader::GouraudShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light)
	:IShader(model, mTrans, cTrans, proTrans, vTrans, light) {
}
GouraudShader::~GouraudShader() {
}

void GouraudShader::vertex(int index, Vec3f*& triangleV) {
	// �ڶ���׶μ������������Ĺ���ֵ
	std::vector<int> face = model->faceVertex(index);
	std::vector<int> faceNormal = model->faceNormal(index);

	for (int j = 0; j < 3; j++) {
		Vec3f v = model->vert(face[j]);
		// ���㶥�㷨����
		vertex_normals[j] = model->normal(faceNormal[j]) * -1;
		
		// ���ݶ��㷨���� ���������
		diffuse[j] = light_dir * vertex_normals[j];

		Vec3f light_reflect = light_dir.reflect(vertex_normals[j]);
		Vec3f h = light_reflect + light_dir;
		Vec3f h_norm = h.normalize();
		specular[j] = std::pow(h_norm * vertex_normals[j], 2);
		
		// ������������ ��Ļ����
		world_coords[j] = MatrixToVec(ProjectionTrans * CameraTrans * ModelTrans * VecToMatrix(v));
		screen_coords[j] = MatrixToVec(ViewTrans * VecToMatrix(world_coords[j]));
	}
	triangleV = screen_coords;
}

bool GouraudShader::fragment(Vec3f bc_screen, Color& color) {
	float diffuse_value = diffuse[0] * bc_screen[0] + diffuse[1] * bc_screen[1] + diffuse[2] * bc_screen[2];
	float specular_value = specular[0] * bc_screen[0] + specular[1] * bc_screen[1] + specular[2] * bc_screen[2];
	float intensity = diffuse_value + specular_value;
	color = Color(intensity * 255, intensity * 255, intensity * 255, 1);
	return true;
}

// BlinnPhongShader
// 
BlinnPhongShader::BlinnPhongShader(Model* model, Matrix mTrans, Matrix cTrans, Matrix proTrans, Matrix vTrans, Vec3f light) 
	:IShader(model, mTrans, cTrans, proTrans, vTrans, light){
}
BlinnPhongShader::~BlinnPhongShader() {
}

void BlinnPhongShader::vertex(int index, Vec3f*& triangleV)  {
	std::vector<int> face = model->faceVertex(index);
	std::vector<int> faceNormal = model->faceNormal(index);
	std::vector<int> faceTexture = model->faceTexture(index);

	for (int j = 0; j < 3; j++) {
		Vec3f v = model->vert(face[j]);
		// ��ȡÿ������ķ�����
		// NOTICE: �ƺ�Ŀǰģ�͵ķ������ķ����Ƿ��ģ�����Ҫ�� -1
		vertex_normals[j] = model->normal(faceNormal[j]) * -1;
		// ��ȡÿ�������uv����
		uv_coords[j] = model->texels(faceTexture[j]);
		// ������������ ��Ļ����
		world_coords[j] = MatrixToVec(ProjectionTrans * CameraTrans * ModelTrans * VecToMatrix(v));
		screen_coords[j] = MatrixToVec(ViewTrans * VecToMatrix(world_coords[j]));
	}
	triangleV = screen_coords;
}

bool BlinnPhongShader::fragment(Vec3f bc_screen, Color& color)  {
	// ���������ֵ������
	Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];

	// ���������ֵ����ɫ
	// ������ Ld = kd * (I / r^2) * max(0, n * light_dir);
	// �߹�   Ls = ks * (I / r^2) * max(0, n * bisector(light_reflect, light_dir));
	Vec3f h = (light_dir.reflect(vertexNormal) + light_dir).normalize();
	float specular = std::pow(h * vertexNormal, 4);
	float diffuse = light_dir * vertexNormal;
	float intensity = diffuse + specular;

	// ��������ӳ��
	//Vec3f texColor1 = model->getTextureTest().getPixelVal(uv_coords[0].x, uv_coords[0].y);
	Vec3f texColor1 = model->getDiffuse(uv_coords[0].x, uv_coords[0].y);
	Vec3f texColor2 = model->getDiffuse(uv_coords[1].x, uv_coords[1].y);
	Vec3f texColor3 = model->getDiffuse(uv_coords[2].x, uv_coords[2].y);

	Vec3f texColor = texColor1 * bc_screen[0] + texColor2 * bc_screen[1] + texColor3 * bc_screen[2];

	//std::cout << texColor << std::endl;
	float r = intensity * texColor[0];
	float g = intensity * texColor[1];
	float b = intensity * texColor[2];
	color = Color(r, g, b, 1);
	return true;
}
