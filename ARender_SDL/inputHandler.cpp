#include "inputHandler.h"

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

bool InputHandler::initInputHandler(SceneHandler& sceneHandler) {
	this->sceneHandler = &sceneHandler;
	
	// �������� RelativeMode
	// While the mouse is in relative mode, the cursor is hidden, 
	// the mouse position is constrained to the window, 
	// and SDL will report continuous relative mouse motion 
	// even if the mouse is at the edge of the window.
	bool success = !SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);

	std::cout << "-inputHandler ����������RelativeMode!" << std::endl;
	return success;
}

void InputHandler::endInputHandler() {

}


void InputHandler::processInput(bool& done, unsigned int deltaT) {
	SDL_Event sdlEvent;
	// ��ȡ��ǰ���¼�
	while (SDL_PollEvent(&sdlEvent)) {

		if (sdlEvent.type == SDL_QUIT) {
			// TODO: ��ֹ����
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
		// SDL�¼�: ��������
		// Key 1-6: �л�����
		// Key ESC: �ر�Ӧ��
		// Key WASD QE: �����ӽ��ƶ���ת
		// Key R: ���������
		// Key Tab: �л����orbitģʽ

		switch (event->key.keysym.sym) {
			// �ƶ��ӽ�
		case SDLK_a:
			std::cout << "������SDLK a" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(-0.25f, 0, 0);
			break;
		case SDLK_s:
			std::cout << "������SDLK s" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, -0.25f, 0);
			break;
		case SDLK_d:
			std::cout << "������SDLK d" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0.25f, 0, 0);
			break;
		case SDLK_w:
			std::cout << "������SDLK w" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, 0.25f, 0);
			break;

			// ��ת�ӽ�
		case SDLK_q:
			std::cout << "������SDLK q" << std::endl;
			sceneHandler->getCurCamera()->RotateCamera(0, -5, 0);
			break;
		case SDLK_e:
			std::cout << "������SDLK e" << std::endl;
			sceneHandler->getCurCamera()->RotateCamera(0, 5, 0);
			break;

			// �����ӽ�
		case SDLK_z:
			std::cout << "������SDLK z" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, 0, 1.f);
			break;
		case SDLK_c:
			std::cout << "������SDLK c" << std::endl;
			sceneHandler->getCurCamera()->MoveCamera(0, 0, -1.f);
			break;

			// reset
		case SDLK_r:
			break;

			// �л�����
		case SDLK_1:
			std::cout << "������SDLK 1" << std::endl;
			sceneHandler->switchScene(1);
			break;
		case SDLK_2:
			std::cout << "������SDLK 2" << std::endl;
			sceneHandler->switchScene(2);
			break;
		case SDLK_3:
			std::cout << "������SDLK 3" << std::endl;
			sceneHandler->switchScene(3);
			break;
		case SDLK_4:
			std::cout << "������SDLK 3" << std::endl;
			sceneHandler->switchScene(4);
			break;

			// �˳�����
		case SDLK_ESCAPE:
			done = true;
			break;
		}

	}
	else if (event->type == SDL_MOUSEMOTION) {
		// SDL�¼�: ����ƶ�

	}
	else if (event->type == SDL_MOUSEWHEEL) {
		// SDL�¼�: �����ֹ���

	}
}
