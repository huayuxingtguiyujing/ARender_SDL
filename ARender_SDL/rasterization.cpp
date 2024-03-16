#include "rasterization.h"

#include "renderHandler.h"
#include "displayHandler.h"


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

	bool ans = (result1 >= 0 && result2 >= 0 && result3 >= 0)
		|| (result1 <= 0 && result2 <= 0 && result3 <= 0);

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
		// ������ӽ��㣬ʹ����������ļ��㹫ʽ (����ֵΪ Vec3f��������������٤��)
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

void Rasterization::triangle(Vec3f* world_triangleV, Vec3f* screen_triangleV, Vec3f* vertex_normals, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, 
	Buffer<float>* zBuffer_SSAA, Buffer<Uint32>* pixelBuffer_SSAA, Vec3f lightDir, float width, float height,const SDL_PixelFormat* MapFormat) {
	BoundingBox boundBox = BoundingBox(screen_triangleV, width, height);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ screen_triangleV[0], screen_triangleV[1], screen_triangleV[2] };

	// ��ƬԪ �ķ�����������ƬԪ����ɫ����Ҫʹ�ò�ֵ��ÿ�����ص���ɫ����ע�ʹ˶�
	Vec3f n = (world_triangleV[2] - world_triangleV[0]) ^ (world_triangleV[1] - world_triangleV[0]);
	n.normalize();
	float faceIntensity = lightDir * n;
	Color faceColor = Color(faceIntensity * 255, faceIntensity * 255, faceIntensity * 255, 1);
	
	// SSAA ����
	float pixel_size_sm = 1.0 / RenderHandler::SSAA_W;
	float start_point = pixel_size_sm / 2.0;

	int ssaa_height = height * zBuffer_SSAA->mHeight;
	int ssaa_width = width * zBuffer_SSAA->mWidth;

	// �����߽�� ����ÿ������
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			float r = 0;
			float g = 0;
			float b = 0;
			
			// ����MSAA �����ж�������������������
			int count = 0;
			int maxCount = 4;

			// �Ƿ������ص�ͨ����ȼ��
			bool flag = false;

			for (float i = 0; i < 1.0; i += pixel_size_sm) {
				for (float j = 0; j < 1.0; j += pixel_size_sm) {
					if (insideTriangle(P.x + i, P.y + j, rec)) {
						count++;

						// ����SSAA��Ҫ����4�����صĵ�
						Vec3f recP = Vec3f(P.x + i , P.y + j , P.z);

						// ��ȡ�������� [alpha, beta, gama]
						Vec3f bc_screen = getBarycentric(screen_triangleV[0], screen_triangleV[1], screen_triangleV[2], recP);
						if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
							continue;
						}

						// ���������ֵ������
						// ERROR: �����⣡
						Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];
						//// ��ƬԪƽ��ķ����� (old)
						//Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
						//n.normalize();

						// ���������ֵ����ɫ
						// ���ղ��ֽ������������� 
						// Ld = kd * (I / r^2) * max(0, n * light_dir);
						float intensity = lightDir * vertexNormal;
						r += intensity * 255 / 4;
						g += intensity * 255 / 4;
						b += intensity * 255 / 4;
						
						// ���������ֵ��z
						P.z = 0;
						float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;

						// z-buffer�ж�
						/*if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
							(*zBuffer)(P.x, P.y) = z_interpolated;
						}*/
						if ((float)z_interpolated > (*zBuffer_SSAA)(P.x + i, P.y + j)) {
							(*zBuffer_SSAA)(P.x + i, P.y + j) = z_interpolated;
							flag = true;
						}
					}
				}
			}

			/*if (insideTriangle(P.x + 0.5f, P.y + 0.5f, rec)) {
				// ����SSAA��Ҫ����4�����صĵ�
				//Vec3f recP = Vec3f(P.x + i , P.y + j , P.z);

				// ��ȡ�������� [alpha, beta, gama]
				Vec3f bc_screen = getBarycentric(screen_triangleV[0], screen_triangleV[1], screen_triangleV[2], P);
				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				// ���������ֵ������
				// ERROR: �����⣡
				Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];

				// ���������ֵ����ɫ
				float intensity = lightDir * vertexNormal;
				r = intensity * 255 * count / 4;
				g = intensity * 255 * count / 4;
				b = intensity * 255 * count / 4;
				// SSAA / MSAA ��ƽ����ɫ
				Color color = Color(r, g, b, 1);

				// ���������ֵ��

				// ���������ֵ��z
				P.z = 0;
				float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;

				// z-buffer�ж�
				if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
					(*zBuffer)(P.x, P.y) = z_interpolated;
					(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
				}

				//// SSAA �汾 ��ɾ
				//if ((float)z_interpolated > (*zBuffer_SSAA)(P.x + i, P.y + j)) {
				//	(*zBuffer_SSAA)(P.x + i, P.y + j) = z_interpolated;
				//	(*pixelBuffer_SSAA)(P.x + i, P.y + j) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
				//	(*pixelBuffer)(P.x, P.y) += (*pixelBuffer_SSAA)(P.x + i, P.y + j);
				//}
			}*/

			//Color color = Color(r, g, b, 1);
			//// ���������ֵ��z
			//P.z = 0;
			//float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;
			//// z-buffer�ж�
			//if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
			//	(*zBuffer)(P.x, P.y) = z_interpolated;
			//	(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			//}
			
			// ����������˱�Ե���� ������ ��Ȼ��棬��ֹ�ڱ�
			//if (count < maxCount) {
			//	(*zBuffer)(P.x, P.y) = std::numeric_limits<float>::min();
			//}
			if (flag) {
				// SSAA / MSAA ��ƽ����ɫ
				Color color = Color(r, g, b, 1);
				(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			}
		}
	}

}

// ��ǰʹ��
void Rasterization::triangle(Vec3f* triangleV, IShader &shader, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, const SDL_PixelFormat* MapFormat) {
	// ���ɱ߽��
	BoundingBox boundBox = BoundingBox(triangleV, DisplayHandler::screenWidth, DisplayHandler::screenHeight);

	Vec3f P;
	Color color;

	// �����߽�� ����ÿ������
#pragma omp parallel for 
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			
			// ��ȡ�������� [alpha, beta, gama]
			Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);
			P.z = 0;
			//bc_screen = bc_screen * (1 / (bc_screen.x + bc_screen.y + bc_screen.z));
			float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;
			
			// ��������: 1.������������; 2.zbuffer���δͨ��
			// NOTICE: �ڱ������Ѿ������ԭ����ʵ�ܼ򵥣�������������ʱ�����ھ��ȶ�ʧ��������ԭ��
			// �ڱ�Ե�ϵĵ����������ֵС��0��ʵ��Ӧ�õ���0�����ʶ�δ����Ⱦ��
			if (bc_screen.x < -1e-1 || bc_screen.y < -1e-1 || bc_screen.z < -1e-1) {
				continue;
			}
			//std::cout << "test" << std::endl;
			/*if (bc_screen.x < -1e-2 || bc_screen.y < -1e-2 || bc_screen.z < -1e-2) {
				continue;
			}*/
			if (z_interpolated < (*zBuffer)(P.x, P.y)) {
				continue;
			}

			//std::cout << "test" << std::endl;

			// �ɼ��Լ��
			bool visible = shader.fragment(bc_screen, color);
			if (visible) {
				(*zBuffer)(P.x, P.y) = z_interpolated;
				(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			}
		}
	}
}

void Rasterization::triangle(Vec3f* triangleV, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, Color color, const SDL_PixelFormat* MapFormat) {
	// ���ɱ߽��
	BoundingBox boundBox = BoundingBox(triangleV, DisplayHandler::screenWidth, DisplayHandler::screenHeight);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ triangleV[0], triangleV[1], triangleV[2] };

	// �����߽�� ����ÿ������
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			// ��ȡ�������� [alpha, beta, gama]
			Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);
			float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;

			// ��������: 1.������������; 2.zbuffer���δͨ��
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 || z_interpolated < (*zBuffer)(P.x, P.y)) {
				continue;
			}

			(*zBuffer)(P.x, P.y) = z_interpolated;
			(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
		}
	}
}

