#pragma once

#include "model.h"
#include "camera.h"
#include "scene.h"

#include <vector>
#include <queue>
#include "math.h"

class SceneHandler
{
public:
	SceneHandler();
	~SceneHandler();

	bool initSceneHandler();
	void endSceneHandler();

	Scene* getCurScene();
	Model* getCurModel();
	std::vector<Model*> getAllModel();
	std::vector<Object*> getAllObject();
	std::vector<Light*> getAllLights();
	Camera* getCurCamera();

	void switchScene(int index);
	void loadScene(std::string sceneName);
	void saveScene(std::string sceneName);

private :

	Scene* curScene;

	Model* model;

	std::vector<Model*> visibleModels;
	std::vector<Model*> allSceneModels;

	// �����еļ�ģ�� (Բ�� ������ ƽ���)
	std::vector<Object*> allSceneObjects;
	std::vector<Light*> allSceneLights;
};

