#include "sceneHandler.h"

SceneHandler::SceneHandler() {
}
SceneHandler::~SceneHandler() {
}

bool SceneHandler::initSceneHandler() {

	visibleModels = std::vector<Model*>();
	allSceneModels = std::vector<Model*>();

	allSceneObjects = std::vector<Object*>();

	// ���� ����
	//loadScene("african_head");
	loadScene("chest");
	//loadScene("student1");
	//loadScene("teapot");
	//loadScene("weapon");

	// ���ؼ�ģ��
	allSceneObjects.push_back(new SphereObj(Vec3f(400, 350, 100), 60, Diffuse_Glossy));
	//allSceneObjects.push_back(new SphereObj(Vec3f(50, 50, 50), 15));
	//allSceneObjects.push_back(new SphereObj(Vec3f(200, 200, 5), 60));
	//allSceneObjects.push_back(new SphereObj(Vec3f(300, 300, 50), 70));

	Vec3f a = Vec3f(200, 200, 200);
	Vec3f b = Vec3f(600, 200, 200);
	Vec3f c = Vec3f(300, 400, 250);
	Vec3f d = Vec3f(500, 400, 250);
	allSceneObjects.push_back(new MeshTriangleObj(b, a, c, Diffuse_Glossy));
	allSceneObjects.push_back(new MeshTriangleObj(b, c, d, Diffuse_Glossy));

	// ���ع���
	//allSceneLights.push_back(new Light(Vec3f(0, 0, 0), Vec3f(0, 0, -1)));
	allSceneLights.push_back(new Light(Vec3f(0, 0, 0), Vec3f(1, 0, 0)));


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
		loadScene("chest");
	}
	else if (index == 3) {
		loadScene("student1");
	}
	else if (index == 4) {
		loadScene("teapot");
	}
	else if (index == 5) {
		loadScene("weapon");
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
		else if (key == "t") {
			// ��ģ�͵� transform
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

std::vector<Object*> SceneHandler::getAllObject() {
	return allSceneObjects;
}

std::vector<Light*> SceneHandler::getAllLights() {
	return allSceneLights;
}

// ��ȡ�����������
Camera* SceneHandler::getCurCamera() {
	return curScene->getCamera();
}