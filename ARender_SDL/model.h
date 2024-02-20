#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "geometry.h"
#include "texture.h"
#include "tgaImage.h"

class Model
{

public:
	Model(std::string modelName, std::string sceneName);
	~Model();

	int nverts();
	int nfaces();

	// ���ʶ�������
	Vec3f vert(int index);
	Vec3f normal(int index);
	Vec3f texels(int index);

	// ����ƬԪ����
	std::vector<int> faceVertex(int index);
	std::vector<int> faceNormal(int index);
	std::vector<int> faceTexture(int index);

	Texture getTextureTest();
	Vec3f getDiffuse(float u, float v);
	
private:
	std::vector<Vec3f> verts_;				// ���еĶ���
	std::vector<Vec3f> normals_;			// ����ƬԪ�ķ�����
	std::vector<Vec3f> texels_;				// ����ӳ������

	std::vector<std::vector<int> > facesVertex_;	// ���е�ƬԪ
	std::vector<std::vector<int> > facesTexture_;
	std::vector<std::vector<int> > facesNormal_;

	// ����
	Texture textureTest;
	TGAImage diffuseTexture;

	// ��������
	std::vector<std::string> splitStr(std::string& str, char deli);
	std::string getStrFileName(std::string filename);
};

