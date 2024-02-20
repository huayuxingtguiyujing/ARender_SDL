#include "Scene.h"

Scene::Scene() {
	camera = new Camera();


}

Scene::~Scene() {
	delete camera;

	//for (auto model : allSceneModels) {

	//}
}

Camera* Scene::getCamera() {
	return camera;
}