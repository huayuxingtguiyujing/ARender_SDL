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

	// 访问 模型数据
	Vec3f vert(int i);
	std::vector<int> face(int idx);

private:
	std::vector<Vec3f> verts_;				// 所有的顶点
	std::vector<Vec3f> normals_;			// 所有片元的法向量
	std::vector<Vec3f> texels_;				// 纹理映射坐标

	std::vector<std::vector<int> > facesVertex_;	// 所有的片元
	std::vector<std::vector<int> > facesTexture_;
	std::vector<std::vector<int> > facesNormal_;

	// 辅助方法
	std::vector<std::string> splitStr(std::string & str, char deli);
};

