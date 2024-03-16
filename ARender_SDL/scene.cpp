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


<<<<<<< HEAD
// Light 
Light::Light() {
	startPos = Vec3f(0, 0, 0);
	lightDir = Vec3f(0, 0, 0);
=======
// Light -¹âÕÕ
Light::Light() {
	startPos = Vec3f(0,0,0);
	lightDir = Vec3f(0,0,0);
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
}

Light::Light(Vec3f s, Vec3f l) {
	startPos = s;
	lightDir = l;
}

Light::~Light() {

<<<<<<< HEAD
}
=======
}
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
