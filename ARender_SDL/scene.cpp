#include "Scene.h"

Scene::Scene() {
	camera = new Camera();
}

Scene::~Scene() {
	delete camera;
}

Camera* Scene::getCamera() {
	return camera;
}