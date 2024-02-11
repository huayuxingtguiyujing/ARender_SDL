#include "apllication.h"

Apllication::Apllication() {

}
Apllication::~Apllication() {

}

bool Apllication::initApp() {
	if (!displayHandler.initDisplayHandler()) {
		std::cout << "displayHandler 初始化失败!" << std::endl;
		return false;
	}
	if (!sceneHandler.initSceneHandler()) {
		std::cout << "sceneHandler 初始化失败!" << std::endl;
		return false;
	}
	if (!inputHandler.initInputHandler(sceneHandler)) {
		std::cout << "inputHandler 初始化失败!" << std::endl;
		return false;
	}
	if (!renderHandler.initRenderHanlder(DisplayHandler::screenWidth, DisplayHandler::screenHeight, &sceneHandler)) {
		std::cout << "renderHandler 初始化失败!" << std::endl;
		return false;
	}
	
	std::cout << "初始化应用程序成功!" << std::endl;
	return true;
}

void Apllication::runApp() {

	bool cancelFlag = false;

	float height = DisplayHandler::screenHeight;
	float width = DisplayHandler::screenWidth;

	// 运行时间戳
	float deltaTime = 1;
	float startTime;
	float endTime;

	while (!cancelFlag) {
		startTime = SDL_GetTicks();
		// TODO: 所有模块 都放置在此处

		inputHandler.processInput(cancelFlag, deltaTime);

		renderHandler.clearBuffers();

		
		// 测试绘制三角形
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

		// 加载模型
		renderHandler.renderModel2(width, height, displayHandler.getSDLFormat());

		// 刷新到屏幕上
		displayHandler.swapBuffers(renderHandler.getScreenBuffer());

		endTime = SDL_GetTicks();
		deltaTime = endTime - startTime;
		//std::cout << "cost time: " << deltaTime << std::endl;
	}
}

void Apllication::endApp() {

}

