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

	// 访问顶点数据
	Vec3f vert(int index);
	Vec3f normal(int index);
	Vec3f texels(int index);

	// 访问片元数据
	std::vector<int> faceVertex(int index);
	std::vector<int> faceNormal(int index);
	std::vector<int> faceTexture(int index);

	Texture getTextureTest();
	Vec3f getDiffuse(float u, float v);
	
private:
	std::vector<Vec3f> verts_;				// 所有的顶点
	std::vector<Vec3f> normals_;			// 所有片元的法向量
	std::vector<Vec3f> texels_;				// 纹理映射坐标

	std::vector<std::vector<int> > facesVertex_;	// 所有的片元
	std::vector<std::vector<int> > facesTexture_;
	std::vector<std::vector<int> > facesNormal_;

	// 纹理
	Texture textureTest;
	TGAImage diffuseTexture;

	// 辅助方法
	std::vector<std::string> splitStr(std::string& str, char deli);
	std::string getStrFileName(std::string filename);
};

