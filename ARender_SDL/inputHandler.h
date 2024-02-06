#pragma once
#include <SDL.h>
#include <iostream>
#include "sceneHandler.h"
#include "camera.h"
class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	bool initInputHandler(SceneHandler& sceneHandler);
	void endInputHandler();

	void processInput(bool& done, unsigned int deltaT);

private:

	SceneHandler* sceneHandler;
	
	void processInputEvent(SDL_Event* event, bool& done, unsigned int deltaT);

};

