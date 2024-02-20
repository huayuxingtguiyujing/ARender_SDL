#include "sceneHandler.h"

SceneHandler::SceneHandler() {
}
SceneHandler::~SceneHandler() {
}

bool SceneHandler::initSceneHandler() {

	//model = new Model("african_head.obj", "african_head");

	visibleModels = std::vector<Model*>();
	allSceneModels = std::vector<Model*>();

	// ����Ĭ�ϳ���
	loadScene("african_head");

	curScene = new Scene();

	return true;
}

void SceneHandler::endSceneHandler() {
	delete curScene;
	delete model;
}

// �л�����
void SceneHandler::switchScene(int index) {
	if (index == 1) {
		loadScene("african_head");
	}
	else if (index == 2) {
		loadScene("african_head");
	}
	else if (index == 3) {
		loadScene("african_head");
	}
}

// ���س���
void SceneHandler::loadScene(std::string sceneName) {
	// ���֮ǰ������ģ��
	int n = allSceneModels.size();
	for (int i = 0; i < n; i++)
	{
		delete allSceneModels[i];
	}

	// ��ȡ���������ļ���Ӧ��������е�ģ���ļ�������Transform���ԣ������ƹ��
	std::ifstream in;
	std::string configFilePath = "scene/" + sceneName + "/config.txt";
	in.open(configFilePath, std::ifstream::in);
	if (in.fail()) {
		std::cout << "SceneHandler: �򿪳��������ļ�ʧ�ܣ���������:" << sceneName << std::endl;
		return;
	}

	std::string line, key;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		iss >> key;
		if (key == "m") {
			// ��ģ���ļ�,����֮,�����뵽��Ⱦ������
			iss >> key;
			Model* model = new Model(key + ".obj", sceneName);
			//eg: model = new Model("african_head.obj", "african_head");
			allSceneModels.push_back(model);
		}
		else if (key == "l") {
			// �ǳ����ƹ�

		}
	}

	std::cout << "SceneHandler: Load scene successfully: " << sceneName << std::endl;
}

// ���泡��
void SceneHandler::saveScene(std::string sceneName) {

}

// ������ get ����

Scene* SceneHandler::getCurScene() {
	return curScene;
}

// ��ȡ��ǰ��������
Model* SceneHandler::getCurModel() {
	return model;
}

std::vector<Model*> SceneHandler::getAllModel() {
	return allSceneModels;
}

// ��ȡ�����������
Camera* SceneHandler::getCurCamera() {
	return curScene->getCamera();
}