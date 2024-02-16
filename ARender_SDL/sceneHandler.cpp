#include "sceneHandler.h"

SceneHandler::SceneHandler() {

}
SceneHandler::~SceneHandler() {

}

bool SceneHandler::initSceneHandler() {
	model = new Model("obj/african_head.obj");

	// 

	curScene = new Scene();

	return true;
}

void SceneHandler::endSceneHandler() {
	delete curScene;
	delete model;
}

// 场景的 get 方法

// 获取当前场景物体
Model* SceneHandler::getCurModel() {
	return model;
}

// 获取场景的摄像机
Camera* SceneHandler::getCurCamera() {
	return curScene->getCamera();
}