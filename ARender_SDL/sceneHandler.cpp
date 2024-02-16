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

// ������ get ����

// ��ȡ��ǰ��������
Model* SceneHandler::getCurModel() {
	return model;
}

// ��ȡ�����������
Camera* SceneHandler::getCurCamera() {
	return curScene->getCamera();
}