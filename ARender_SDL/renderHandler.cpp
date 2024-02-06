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
void RenderHandler::renderModel(Model* model, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, float width, float height, const SDL_PixelFormat* format) {
	Vec3f light_dir(0, 0, -1);

	// MVP变换
	//Matrix translate = Matrix::getTranslate(1,0,0);
	//Matrix rotation = Matrix::getRotationZ(45);
	//Matrix zoom = Matrix::getZoom();

	// 摄像机变换
	Matrix cameraMat = sceneHandler->getCurCamera()->getCameraTrans();
	std::cout << "Cur Camera Matrix:" << std::endl << cameraMat << std::endl;
	// 视口变换矩阵
	Matrix viewMat = Matrix::getViewport(0, 0, width, height);

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec3f screen_coords[3];
		Vec3f world_coords[3];

		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			// 计算世界坐标
			world_coords[j] = Rasterization::MatrixToVec(cameraMat * Rasterization::VecToMatrix(v));
			// 计算屏幕坐标
			screen_coords[j] = Rasterization::MatrixToVec(viewMat* Rasterization::VecToMatrix(world_coords[j]));
		}

		// 求片元平面的法向量
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();

		// 光照部分仅计算了漫反射 
		// Ld = kd * (I / r^2) * max(0, n * light_dir);
		float intensity = n * light_dir;

		if (intensity > 0) {
			float r = intensity * 255;
			float g = intensity * 255;
			float b = intensity * 255;
			Color color = Color(r, g, b, 1);
			Rasterization::triangle(screen_coords, zBuffer, pixelBuffer,
				color, width, height, format);
		}
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
			(*zBuffer)(i, j) = -std::numeric_limits<float>::max();
		}
	}

	pixelBuffer = new Buffer<Uint32>(w, h, new Uint32[w * h]);
	if (pixelBuffer == nullptr) {
		std::cout << "创建pixel-buffer缓冲失败" << std::endl;
		return false;
	}

	return true;
}

Buffer<Uint32>* RenderHandler::getScreenBuffer() {
	return pixelBuffer;
}

Buffer<float>* RenderHandler::getZBuffer() {
	return zBuffer;
}

void RenderHandler::clearBuffers() {
	zBuffer->clear();

	pixelBuffer->clear();
}

