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
	// ���߷���
	light_dir = light;
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
		// NOTICE: �ƺ�Ŀǰ�������ķ����Ƿ��ģ�����Ҫ�� -1
		vertex_normals[j] = model->normal(faceNormal[j]) * -1;
		// ��ȡÿ�������uv����,��Ӧ����ɫ
		uv_coords[j] = model->texels(faceTexture[j]);
		
		//uv_color[j] = model->getTextureTest().getPixelVal(uv_coords[j].x, uv_coords[j].y);
		// ������������
		world_coords[j] = MatrixToVec(CameraTrans * ModelTrans * VecToMatrix(v));
		// ������Ļ����
		screen_coords[j] = MatrixToVec(ViewTrans * VecToMatrix(world_coords[j]));
	}

	triangleV = screen_coords;
}

bool BlinnPhongShader::fragment(Vec3f bc_screen, Color& color)  {
	// ���������ֵ������
	Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];

	// ���������ֵ����ɫ
	// ���ղ��ֽ������������� Ld = kd * (I / r^2) * max(0, n * light_dir);
	float intensity = light_dir * vertexNormal;

	// ��������ӳ��
	//Vec3f texColor1 = model->getTextureTest().getPixelVal(uv_coords[0].x, uv_coords[0].y);
	//Vec3f texColor2 = model->getTextureTest().getPixelVal(uv_coords[1].x, uv_coords[1].y);
	//Vec3f texColor3 = model->getTextureTest().getPixelVal(uv_coords[2].x, uv_coords[2].y);

	//std::cout << "Can reach here!" << std::endl;

	Vec3f texColor1 = model->getDiffuse(uv_coords[0].x, uv_coords[0].y);
	Vec3f texColor2 = model->getDiffuse(uv_coords[1].x, uv_coords[1].y);
	Vec3f texColor3 = model->getDiffuse(uv_coords[2].x, uv_coords[2].y);

	//std::cout << "Can reach here!!" << std::endl;
	
	Vec3f texColor = texColor1 * bc_screen[0] + texColor2 * bc_screen[1] + texColor3 * bc_screen[2];

	//std::cout << texColor << std::endl;
	// static_cast<unsigned int>(value)

	// * texColor[0]
	//float r = intensity * 255;
	//float g = intensity * 255;
	//float b = intensity * 255;
	float r = intensity * texColor[0];
	float g = intensity * texColor[1];
	float b = intensity * texColor[2];
	color = Color(r, g, b, 1);
	//color = Color(255, 255, 255, 1);

	return true;
}