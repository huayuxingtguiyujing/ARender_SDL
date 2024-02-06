#pragma once

#include "sceneHandler.h"
#include "displayHandler.h"
#include "renderHandler.h"
#include "inputHandler.h"

#include "color.h"
#include "rasterization.h"

#include "model.h"

#include <iostream>

class Apllication
{
public:
	Apllication();
	~Apllication();

	bool initApp();
	void runApp();
	void endApp();

private:
	SceneHandler sceneHandler;
	RenderHandler renderHandler;
	InputHandler inputHandler;
	DisplayHandler displayHandler;

	Model* model;
};

