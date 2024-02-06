#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "vector.h"

class Model
{

public:
	Model(const char* filename);
	~Model();

	int nverts();
	int nfaces();

	// ���� ģ������
	Vec3f vert(int i);
	std::vector<int> face(int idx);

private:
	std::vector<Vec3f> verts_;				// ���еĶ���
	std::vector<Vec3f> normals_;			// ����ƬԪ�ķ�����
	std::vector<Vec3f> texels_;				// ����ӳ������

	std::vector<std::vector<int> > facesVertex_;	// ���е�ƬԪ
	std::vector<std::vector<int> > facesTexture_;
	std::vector<std::vector<int> > facesNormal_;

	// ��������
	std::vector<std::string> splitStr(std::string & str, char deli);
};

