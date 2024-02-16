#include "inputHandler.h"

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

bool InputHandler::initInputHandler(SceneHandler& sceneHandler) {
	this->sceneHandler = &sceneHandler;
	
	// 让鼠标进入 RelativeMode
	// While the mouse is in relative mode, the cursor is hidden, 
	// the mouse position is constrained to the window, 
	// and SDL will report continuous relative mouse motion 
	// even if the mouse is at the edge of the window.
	bool success = !SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);

	std::cout << "-inputHandler 已让鼠标进入RelativeMode!" << std::endl;
	return success;
}

void InputHandler::endInputHandler() {

}


void InputHandler::processInput(bool& done, unsigned int deltaT) {
	SDL_Event sdlEvent;
	// 获取当前的事件
	while (SDL_PollEvent(&sdlEvent)) {

		if (sdlEvent.type == SDL_QUIT) {
			// TODO: 中止程序
			done = false;
			return;
		}
		else {
			processInputEvent(&sdlEvent, done, deltaT);
		}
	}
}

void InputHandler::processInputEvent(SDL_Event* event, bool& done, unsigned int deltaT) {
	float cameraSpeed = 1;

	if (event->type == SDL_KEYDOWN) {
		// SDL事件: 键盘输入
		// Key 1-6: 切换场景
		// Key ESC: 关闭应用
		// Key WASD QE: 控制视角移动旋转
		// Key R: 重置摄像机
		// Key Tab: 切换相机orbit模式

		switch (event->key.keysym.sym) {
			// 移动视角
		case SDLK_a:
			std::cout << "按下了SDLK a" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(-0.25f, 0, 0);
			break;
		case SDLK_s:
			std::cout << "按下了SDLK s" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, -0.25f, 0);
			break;
		case SDLK_d:
			std::cout << "按下了SDLK d" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0.25f, 0, 0);
			break;
		case SDLK_w:
			std::cout << "按下了SDLK w" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, 0.25f, 0);
			break;

			// 旋转视角
		case SDLK_q:
			std::cout << "按下了SDLK q" << std::endl;
			sceneHandler->getCurCamera()->RotateCamera(0, -25, 0);
			break;
		case SDLK_e:
			std::cout << "按下了SDLK e" << std::endl;
			sceneHandler->getCurCamera()->RotateCamera(0, 25, 0);
			break;

			// reset
		case SDLK_r:
			break;

			// 切换场景
		case SDLK_1:
			std::cout << "按下了SDLK 1" << std::endl;
			break;
		case SDLK_2:
			std::cout << "按下了SDLK 2" << std::endl;
			break;
		case SDLK_3:
			std::cout << "按下了SDLK 3" << std::endl;
			break;

			// 退出程序
		case SDLK_ESCAPE:
			done = true;
			break;
		}

	}
	else if (event->type == SDL_MOUSEMOTION) {
		// SDL事件: 鼠标移动

	}
	else if (event->type == SDL_MOUSEWHEEL) {
		// SDL事件: 鼠标滚轮滚动

	}
}
