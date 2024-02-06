#include "rasterization.h"

const SDL_PixelFormat* Rasterization::mappingFormat(SDL_AllocFormat(PIXEL_FORMAT));

// ����
void Rasterization::line(int x0, int y0, int x1, int y1) {
	for (float t = 0.; t < 1.; t += .01) {
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		// TODO: 
	}
}

void Rasterization::line(Vec3f p0, Vec3f p1) {
	line(p0.x, p0.y, p1.x, p1.y);
}

// trans vector to matrix
Matrix Rasterization::VecToMatrix(Vec3f vec) {
	Matrix m(4, 1);
	m[0][0] = vec.x;
	m[1][0] = vec.y;
	m[2][0] = vec.z;
	m[3][0] = 1.f;
	return m;
}

Vec3f Rasterization::MatrixToVec(Matrix m) {
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

// �жϵ��Ƿ�����������
bool Rasterization::insideTriangle(float x, float y, const std::vector<Vec3f> triangleV)
{
	// ����ʹ��ͬ�򷨺����ķ�ʵ�֣��˴�ʹͬ��
	Vec3f AB = triangleV[1] - triangleV[0];
	Vec3f BC = triangleV[2] - triangleV[1];
	Vec3f CA = triangleV[0] - triangleV[2];

	Vec3f P = Vec3f(x, y, 0);
	Vec3f PA = triangleV[0] - P;
	Vec3f PB = triangleV[1] - P;
	Vec3f PC = triangleV[2] - P;

	float result1 = (PA ^ AB) * (CA ^ AB);
	float result2 = (PB ^ BC) * (AB ^ BC);
	float result3 = (PC ^ CA) * (BC ^ CA);

	bool ans = (result1 > 0 && result2 > 0 && result3 > 0)
		|| (result1 < 0 && result2 < 0 && result3 < 0);

	//std::cout << "point (" << x << "," << y << ") in triangle: " << ans << std::endl;

	// �ж�������˵Ľ���Ƿ������ͬһ����ͬ���ķ��ţ�
	return ans;
}

// ��ȡ�����ε��������� ����(u,v,w)
Vec3f Rasterization::getBarycentric(Vec3f A, Vec3f B, Vec3f C) {
	return Vec3f((A.x + B.x + C.x) / 3, (A.y + B.y + C.y) / 3, (A.z + B.z + C.z) / 3);
}

Vec3f Rasterization::getBarycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// ����������������
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}

	// ���㷨����
	Vec3f u = s[0] ^ (s[1]);

	// ��鷨�����ĵ����������Ƿ�ӽ��㣬����������
	if (std::abs(u.z) > 1e-2) {
		// ������ӽ��㣬ʹ����������ļ��㹫ʽ
		return { 1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z };
	}

	// ����������ĵ����������ӽ��㣬��ʾ�������˻�����������ֵ
	return { -1.0f, 1.0f, 1.0f };
}

// ��������
void Rasterization::triangle(Vec3f* triangleV, float* zbuffer, Color color, float width, float height) {
	BoundingBox boundBox = BoundingBox(triangleV, width, height);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ triangleV[0], triangleV[1], triangleV[2] };
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			// �жϸõ��Ƿ�����������
			if (insideTriangle(P.x, P.y, rec)) {

				// ���������ֵ��z
				Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);

				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				P.z = 0;
				float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;

				// z-buffer�ж�
				int pixelIndex = int(P.x + P.y * width);
				if (z_interpolated > zbuffer[pixelIndex]) {
					zbuffer[pixelIndex] = z_interpolated;
					//image.set(P.x, P.y, color);
				}

			}
			else {
				//std::cout << "this point is inside!" << std::endl;
			}
		}
	}

	//std::cout << "successfully draw triangle!" << std::endl;
}

void Rasterization::triangle(Vec3f* triangleV, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, Color color, float width, float height,const SDL_PixelFormat* MapFormat) {
	BoundingBox boundBox = BoundingBox(triangleV, width, height);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ triangleV[0], triangleV[1], triangleV[2] };

	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			if (insideTriangle(P.x, P.y, rec)) {
				// ���������ֵ��z
				Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);
				// ���������ֵ����ɫ

				// ���������ֵ��


				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				P.z = 0;
				float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;

				// z-buffer�ж�
				if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
					(*zBuffer)(P.x, P.y) = z_interpolated;
					(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
				}
				
			}
		}
	}

}

