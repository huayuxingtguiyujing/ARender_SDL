#pragma once

#include "model.h"
#include "camera.h"
#include "scene.h"
#include "object.h"

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

	// get 方法
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

	// 场景中的模型
	std::vector<Model*> visibleModels;
	std::vector<Model*> allSceneModels;
<<<<<<< HEAD

=======
	
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
	// 场景中的简单模型 (圆形 三角形 平面等)
	std::vector<Object*> allSceneObjects;
	std::vector<Light*> allSceneLights;
};

