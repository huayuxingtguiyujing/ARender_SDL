#pragma once

#include "camera.h"
#include "model.h"

#include<queue>

class Scene
{
public:

	Scene();
	~Scene();

	Camera* getCamera();

private:
	Camera* camera;


};

<<<<<<< HEAD

=======
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
struct Light {
	Vec3f startPos;
	Vec3f lightDir;

	Light();
	Light(Vec3f s, Vec3f l);
	~Light();
};
