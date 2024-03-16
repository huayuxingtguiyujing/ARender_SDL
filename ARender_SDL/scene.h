#pragma once

#include "camera.h"
#include "model.h"

#include<queue>

class Scene
{
public:

	Scene();
	~Scene();

	Camera* getCamera();

private:
	Camera* camera;


};


struct Light {
	Vec3f startPos;
	Vec3f lightDir;

	Light();
	Light(Vec3f s, Vec3f l);
	~Light();
};
