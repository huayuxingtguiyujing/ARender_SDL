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
void RenderHandler::renderModel(float width, float height, const SDL_PixelFormat* format) {
	Vec3f light_dir(0, 0, -1);

	Model* model = sceneHandler->getCurModel();

	// 模型变换
	Matrix translate = Matrix::getTranslate(0,0,0);
	Matrix rotation = Matrix::getRotationZ(0);
	Matrix zoom = Matrix::getZoom(1.);
	Matrix modelTrans = zoom * rotation * translate;

	// 摄像机变换
	Matrix cameraMat = sceneHandler->getCurCamera()->getCameraTrans();
	std::cout << "Cur Camera Matrix:" << std::endl << cameraMat << std::endl;

	// 投影变换
	// ERROR: 投影变换未达到预期效果
	Matrix projectionMat = Matrix::getPersProjection(-1, 1, 1., 45);

	// 视口变换矩阵
	Matrix viewMat = Matrix::getViewport(0, 0, width, height);

	BlinnPhongShader shader = BlinnPhongShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->faceVertex(i);
		std::vector<int> faceNormal = model->faceNormal(i);
		// 顶点数据
		Vec3f vertex_normals[3];
		Vec3f screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			// 获取每个顶点的法向量
			// NOTICE: 似乎目前法向量的方向是反的，所以要乘 -1
			vertex_normals[j] = model->normal(faceNormal[j]) * -1;
			// 计算世界坐标
			world_coords[j] = MatrixToVec(cameraMat * modelTrans * VecToMatrix(v));
			// 计算屏幕坐标
			screen_coords[j] = MatrixToVec(viewMat* VecToMatrix(world_coords[j]));
		}

		//shader.vertex(i, screen_coords);

		//Rasterization::triangle(screen_coords, shader, zBuffer, pixelBuffer);
		Rasterization::triangle(world_coords, screen_coords, vertex_normals, zBuffer, pixelBuffer, SSAA_zBuffer, SSAA_pixelBuffer, light_dir, width, height, format);
	}
}

void RenderHandler::renderModel2(float width, float height, const SDL_PixelFormat* format) {
	Vec3f light_dir(0, 0, -1);

	Model* model = sceneHandler->getCurModel();

	// 模型变换
	Matrix translate = Matrix::getTranslate(0, 0, 0);
	Matrix rotation = Matrix::getRotationZ(0);
	Matrix zoom = Matrix::getZoom(1.);
	Matrix modelTrans = zoom * rotation * translate;

	// 摄像机变换
	Matrix cameraMat = sceneHandler->getCurCamera()->getCameraTrans();
	std::cout << "Cur Camera Matrix:" << std::endl << cameraMat << std::endl;

	// 投影变换 // ERROR: 投影变换未达到预期效果
	Matrix projectionMat = Matrix::getPersProjection(-1, 1, 1., 45);

	// 视口变换矩阵
	Matrix viewMat = Matrix::getViewport(0, 0, width, height);

	// 着色器
	BlinnPhongShader shader = BlinnPhongShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);

	for (int i = 0; i < model->nfaces(); i++) {
		Vec3f* triangleV;
		shader.vertex(i, triangleV);
		Rasterization::triangle(triangleV, shader, zBuffer, pixelBuffer, format);
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
