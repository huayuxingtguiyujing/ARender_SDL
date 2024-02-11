#include "apllication.h"

Apllication::Apllication() {

}
Apllication::~Apllication() {

}

bool Apllication::initApp() {
	if (!displayHandler.initDisplayHandler()) {
		std::cout << "displayHandler ��ʼ��ʧ��!" << std::endl;
		return false;
	}
	if (!sceneHandler.initSceneHandler()) {
		std::cout << "sceneHandler ��ʼ��ʧ��!" << std::endl;
		return false;
	}
	if (!inputHandler.initInputHandler(sceneHandler)) {
		std::cout << "inputHandler ��ʼ��ʧ��!" << std::endl;
		return false;
	}
	if (!renderHandler.initRenderHanlder(DisplayHandler::screenWidth, DisplayHandler::screenHeight, &sceneHandler)) {
		std::cout << "renderHandler ��ʼ��ʧ��!" << std::endl;
		return false;
	}
	
	std::cout << "��ʼ��Ӧ�ó���ɹ�!" << std::endl;
	return true;
}

void Apllication::runApp() {

	bool cancelFlag = false;

	float height = DisplayHandler::screenHeight;
	float width = DisplayHandler::screenWidth;

	// ����ʱ���
	float deltaTime = 1;
	float startTime;
	float endTime;

	while (!cancelFlag) {
		startTime = SDL_GetTicks();
		// TODO: ����ģ�� �������ڴ˴�

		inputHandler.processInput(cancelFlag, deltaTime);

		renderHandler.clearBuffers();

		
		// ���Ի���������
		/*Vec3f A = Vec3f(500, 500, 0);
		Vec3f B = Vec3f(600, 600, 0);
		Vec3f C = Vec3f(500, 600, 0);
		Vec3f trangleV[3] = {A, B, C};
		Color color = Color(1,0,1,1);
		for (int i = 0; i < 3; i++) {
			trangleV[i] = Rasterization::MatrixToVec(viewMat * cameraMat * Rasterization::VecToMatrix(trangleV[i]));
		}

		Rasterization::triangle(trangleV,
			renderHandler.getZBuffer(), renderHandler.getScreenBuffer(),
			color, width, height, displayHandler.getSDLFormat());*/

		// ����ģ��
		renderHandler.renderModel2(width, height, displayHandler.getSDLFormat());

		// ˢ�µ���Ļ��
		displayHandler.swapBuffers(renderHandler.getScreenBuffer());

		endTime = SDL_GetTicks();
		deltaTime = endTime - startTime;
		//std::cout << "cost time: " << deltaTime << std::endl;
	}
}

void Apllication::endApp() {

}

