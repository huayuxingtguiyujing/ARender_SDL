#include "renderHandler.h"

RenderHandler::RenderHandler() {
}

RenderHandler::~RenderHandler() {
}

bool RenderHandler::initRenderHanlder(int w, int h, SceneHandler* sh) {
	if (!createBuffers(w, h)) {
		std::cout << "������Ļ������ʧ��" << std::endl;
		return false;
	}

	sceneHandler = sh;

	return true;
}

void RenderHandler::endRenderHandler() {
	clearBuffers();
}


// ��Ⱦָ����ģ��
void RenderHandler::renderModel(Model* model, float width, float height, const SDL_PixelFormat* format) {
	Vec3f light_dir(0, 0, -1);

	// ģ�ͱ任
	Matrix translate = Matrix::getTranslate(0, 0, 0);
	Matrix rotation = Matrix::getRotationY(0);
	Matrix zoom = Matrix::getZoom(1.);
	Matrix modelTrans = zoom * rotation * translate;

	// ������任
	Camera* cameraRec = sceneHandler->getCurCamera();
	Matrix cameraMat = cameraRec->viewMat;
	std::cout << "Cur Camera Matrix:" << std::endl << cameraMat << std::endl;

	// ͶӰ�任 // ERROR: ͶӰ�任δ�ﵽԤ��Ч��
	// 2.27 ����: ��Ȼû�дﵽԤ��Ч��
	//Matrix projectionMat = Matrix::getPersProjection(-1, 1, 1., 45);
	Matrix projectionMat = cameraRec->projectionMat;

	// �ӿڱ任����
	Matrix viewMat = Matrix::getViewport(0, 0, width, height);

	// ��ɫ��
	BlinnPhongShader shader = BlinnPhongShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);
	//FlatShader shader = FlatShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);
	//GouraudShader shader = GouraudShader(model, modelTrans, cameraMat, projectionMat, viewMat, light_dir);

#pragma omp parallel for 
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

<<<<<<< HEAD

=======
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
// ��Ⱦ������Object (ͨ��ray tracing ���ǹ�դ��)
void RenderHandler::renderObject(Object* object, float width, float height, const SDL_PixelFormat* format) {
}

bool RenderHandler::isIntersect(const Light* light, double& intersection, int& objId, std::vector<Object*> recObj) {

	// ���� ��ʼֵ ȡdouble���ֵ
	double maxNum = std::numeric_limits<double>::max();
	intersection = maxNum;
	double recT = 0;
	for (int i = 0; i < recObj.size(); i++) {
		recT = recObj[i]->intersection(light->startPos, light->lightDir);
		// ����ֵ ������� 0���ұȵ�ǰ�Ľ���λ��С�����¼ �ཻ�� �� �ཻ������ID
		if (recT > 0 && recT < intersection) {
			intersection = recT;
			objId = i;
		}
	}

	return intersection < maxNum;
}

<<<<<<< HEAD
bool RenderHandler::tracing(const Light* lookDir, const Light* light, int reflectNum, float& diffuse, float& specular, std::vector<Object*> recObj) {
=======
bool RenderHandler::tracing(const Light* lookDir, const Light* light, int reflectNum, float & diffuse, float & specular, std::vector<Object*> recObj) {
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
	// ����һֱִ�з���
	reflectNum--;
	if (reflectNum <= 0) {
		return false;
	}

	// �жϷ�������Ƿ��������ཻ
	double intersection;
	int objId;
	bool isHit = isIntersect(lookDir, intersection, objId, recObj);
	if (isHit) {
		// ��û��е� λ�á�������
		Vec3f hitPos = lookDir->startPos + lookDir->lightDir * intersection;
		Vec3f normal = recObj[objId]->getNormal(hitPos);

		float LdotN = (light->lightDir) * normal.normalize();
		LdotN = std::max(0.f, LdotN);

		// ERROR: �����������ˣ�����
		// 1.���������䷽��ι۲췽�������⣬��Ӱ��߹�
		// 2.?
		// �������� �� �߹��� (TODO: ���� ks=0.2 �� kd=0.8)
		diffuse += LdotN * 0.8;
		Vec3f R = lookDir->lightDir + light->lightDir;
		specular += powf(std::max(0.f, lookDir->lightDir * R.normalize()), 3) * 0.2;

		//std::cout << "hit: " << hitPos << std::endl;

		// ������䷽��
		float dirN = lookDir->lightDir * normal;
		Vec3f reflectDir = lookDir->lightDir - normal * 2 * dirN;
		Light* reflect = new Light(hitPos, reflectDir);

		// �ݹ�����ཻ�����ɫ
		tracing(reflect, light, reflectNum, diffuse, specular, recObj);

		delete reflect;
	}

	return true;
}

void RenderHandler::renderAllObj(float width, float height, const SDL_PixelFormat* format) {
	std::vector<Object*> recObj = sceneHandler->getAllObject();
	std::vector<Light*> recLight = sceneHandler->getAllLights();
<<<<<<< HEAD

=======
	
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
	// ���ߵ���ʼ��
	Vec3f eyePos = Vec3f(10, 10, -5000);

	int screenWidth = DisplayHandler::screenWidth;
	int screenHeight = DisplayHandler::screenHeight;

<<<<<<< HEAD
	// TODO: д��BVH������Ӧ�õ�������

=======
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
	// ����ÿ�����ص�
	for (int i = 0; i < screenWidth; i++) {
		for (int j = 0; j < screenHeight; j++) {
			float diffuse = 0;
			float specular = 0;
			for (int k = 0; k < recLight.size(); k++) {
				// ���ɳ�ʼ����
				Vec3f eyeDir = Vec3f(i, j, 0) - eyePos;
				eyeDir = eyeDir.normalize();
				Light* lookAt = new Light(eyePos, eyeDir);

				// whitted style ray tracing
				tracing(lookAt, recLight[k], 3, diffuse, specular, recObj);
<<<<<<< HEAD

=======
				
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
				delete lookAt;
			}

			float intensity = diffuse + specular;
			//std::cout << intensity << std::endl;
			(*pixelBuffer)(i, j) = SDL_MapRGB(format, intensity * 255, intensity * 255, intensity * 255);
		}
	}

	std::cout << "ray tracing object: " << recObj.size() << std::endl;
}

<<<<<<< HEAD

=======
>>>>>>> fd8a9d3cd7bece7f7560f0b9f8b3fa741ba7a417
// ������Ļ���ػ�����
bool RenderHandler::createBuffers(int w, int h) {
	zBuffer = new Buffer<float>(w, h, new float[w * h]);
	if (zBuffer == nullptr) {
		std::cout << "����z-buffer����ʧ��" << std::endl;
		return false;
	}

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			(*zBuffer)(i, j) = std::numeric_limits<float>::min();
		}
	}

	pixelBuffer = new Buffer<Uint32>(w, h, new Uint32[w * h]);
	if (pixelBuffer == nullptr) {
		std::cout << "����pixel-buffer����ʧ��" << std::endl;
		return false;
	}

	// ����SSAA ������
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
