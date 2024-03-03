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


// Light -π‚’’
Light::Light() {
	startPos = Vec3f(0,0,0);
	lightDir = Vec3f(0,0,0);
}

Light::Light(Vec3f s, Vec3f l) {
	startPos = s;
	lightDir = l;
}

Light::~Light() {

}
