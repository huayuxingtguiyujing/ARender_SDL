#include "renderHandler.h"

RenderHandler::RenderHandler() {

}

RenderHandler::~RenderHandler() {

}

bool RenderHandler::initRenderHanlder(int w, int h, SceneHandler* sh) {
	if (!createBuffers(w, h)) {
		std::cout << "创建屏幕缓冲区失败" << std::endl;
		return false;
	}

	sceneHandler = sh;

	return true;
}

void RenderHandler::endRenderHandler() {
	clearBuffers();
}


// 渲染指定的模型
void RenderHandler::renderModel(Model* model, float width, float height, const SDL_PixelFormat* format) {
	Vec3f light_dir(0, 0, -1);

	// 模型变换
	Matrix translate = Matrix::getTranslate(0, 0, 0);
	Matrix rotation = Matrix::getRotationY(0);
	Matrix zoom = Matrix::getZoom(1.);
	Matrix modelTrans = zoom * rotation * translate;

	// 摄像机变换
	Camera* cameraRec = sceneHandler->getCurCamera();
	Matrix cameraMat = cameraRec->viewMat;
	std::cout << "Cur Camera Matrix:" << std::endl << cameraMat << std::endl;

	// 投影变换 // ERROR: 投影变换未达到预期效果
	// 2.27 更新: 依然没有达到预期效果
	//Matrix projectionMat = Matrix::getPersProjection(-1, 1, 1., 45);
	Matrix projectionMat = cameraRec->projectionMat;

	// 视口变换矩阵
	Matrix viewMat = Matrix::getViewport(0, 0, width, height);

	// 着色器
	//BlinnPhongShader shader = BlinnPhongShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);
	//FlatShader shader = FlatShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);
	GouraudShader shader = GouraudShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);

	for (int i = 0; i < model->nfaces(); i++) {
		Vec3f* triangleV;
		shader.vertex(i, triangleV);
		Rasterization::triangle(triangleV, shader, zBuffer, pixelBuffer, format);
	}
}


void RenderHandler::renderAllModel(float width, float height, const SDL_PixelFormat* format) {
	std::vector<Model*> rec = sceneHandler->getAllModel();
	for (int i = 0; i < rec.size(); i++) {
		renderModel(rec[i], width, height, format);
	}
}

// 管理屏幕像素缓冲区
bool RenderHandler::createBuffers(int w, int h) {
	zBuffer = new Buffer<float>(w, h, new float[w * h]);
	if (zBuffer == nullptr) {
		std::cout << "创建z-buffer缓冲失败" << std::endl;
		return false;
	}

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			(*zBuffer)(i, j) = std::numeric_limits<float>::min();
		}
	}

	pixelBuffer = new Buffer<Uint32>(w, h, new Uint32[w * h]);
	if (pixelBuffer == nullptr) {
		std::cout << "创建pixel-buffer缓冲失败" << std::endl;
		return false;
	}

	// 创建SSAA 缓冲区
	SSAA_zBuffer = new Buffer<float>(w * SSAA_W, h * SSAA_H, new float[w * SSAA_W * h * SSAA_H]);
	for (int i = 0; i < w * SSAA_W; i++) {
		for (int j = 0; j < h * SSAA_H; j++) {
			(*SSAA_zBuffer)(i, j) = std::numeric_limits<float>::min();
		}
	}
	SSAA_pixelBuffer = new Buffer<Uint32>(w * SSAA_W, h * SSAA_H, new Uint32[w * SSAA_W * h * SSAA_H]);

	return true;
}

void RenderHandler::clearBuffers() {
	zBuffer->clear();

	SSAA_zBuffer->clear();
	pixelBuffer->clear();
	SSAA_pixelBuffer->clear();
}

Buffer<float>* RenderHandler::getZBuffer() {
	return zBuffer;
}

Buffer<float>* RenderHandler::getZBuffer_SSAA() {
	return SSAA_zBuffer;
}

Buffer<Uint32>* RenderHandler::getScreenBuffer() {
	return pixelBuffer;
}

Buffer<Uint32>* RenderHandler::getScreenBuffer_SSAA() {
	return SSAA_pixelBuffer;
}
