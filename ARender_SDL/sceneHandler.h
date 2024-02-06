#pragma once

#include "model.h"
#include "camera.h"
#include "scene.h"

class SceneHandler
{
public:
	SceneHandler();
	~SceneHandler();

	bool initSceneHandler();
	void endSceneHandler();

	Model* getCurModel();
	Camera* getCurCamera();

private :

	Scene* curScene;

	Model* model;

};

