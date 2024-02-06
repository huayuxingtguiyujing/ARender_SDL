#pragma once

#include "camera.h"

class Scene
{
public:

	Scene();
	~Scene();

	Camera* getCamera();

private:
	Camera* camera;

};

