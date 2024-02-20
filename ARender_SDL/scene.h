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

