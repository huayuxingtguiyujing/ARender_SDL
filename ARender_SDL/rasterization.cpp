#include "rasterization.h"

#include "renderHandler.h"
#include "displayHandler.h"


const SDL_PixelFormat* Rasterization::mappingFormat(SDL_AllocFormat(PIXEL_FORMAT));

// 画线
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

// 判断点是否在三角形内
bool Rasterization::insideTriangle(float x, float y, const std::vector<Vec3f> triangleV)
{
	// 可以使用同向法和重心法实现，此处使同向法
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

	// 判断向量叉乘的结果是否均处于同一方向（同样的符号）
	return ans;
}

// 获取三角形的重心坐标 返回(u,v,w)
Vec3f Rasterization::getBarycentric(Vec3f A, Vec3f B, Vec3f C) {
	return Vec3f((A.x + B.x + C.x) / 3, (A.y + B.y + C.y) / 3, (A.z + B.z + C.z) / 3);
}

Vec3f Rasterization::getBarycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
	// 计算两个辅助向量
	Vec3f s[2];
	for (int i = 2; i--; ) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}

	// 计算法向量
	Vec3f u = s[0] ^ (s[1]);

	// 检查法向量的第三个分量是否接近零，避免除零错误
	if (std::abs(u.z) > 1e-2) {
		// 如果不接近零，使用重心坐标的计算公式 (返回值为 Vec3f（阿法，贝塔，伽马）)
		return { 1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z };
	}

	// 如果法向量的第三个分量接近零，表示三角形退化，返回特殊值
	return { -1.0f, 1.0f, 1.0f };
}

// 画三角形
void Rasterization::triangle(Vec3f* triangleV, float* zbuffer, Color color, float width, float height) {
	BoundingBox boundBox = BoundingBox(triangleV, width, height);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ triangleV[0], triangleV[1], triangleV[2] };
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			// 判断该点是否在三角形内
			if (insideTriangle(P.x, P.y, rec)) {

				// 重心坐标插值求z
				Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);

				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				P.z = 0;
				float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;

				// z-buffer判定
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

	// 求片元 的法向量，并求片元的颜色，如要使用插值求每个像素的颜色，请注释此段
	Vec3f n = (world_triangleV[2] - world_triangleV[0]) ^ (world_triangleV[1] - world_triangleV[0]);
	n.normalize();
	float faceIntensity = lightDir * n;
	Color faceColor = Color(faceIntensity * 255, faceIntensity * 255, faceIntensity * 255, 1);
	
	// SSAA 参数
	float pixel_size_sm = 1.0 / RenderHandler::SSAA_W;
	float start_point = pixel_size_sm / 2.0;

	int ssaa_height = height * zBuffer_SSAA->mHeight;
	int ssaa_width = width * zBuffer_SSAA->mWidth;

	// 遍历边界盒 设置每个像素
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			float r = 0;
			float g = 0;
			float b = 0;
			
			// 采用MSAA 计算有多少子像素在三角形内
			int count = 0;
			int maxCount = 4;

			// 是否有像素点通过深度检测
			bool flag = false;

			for (float i = 0; i < 1.0; i += pixel_size_sm) {
				for (float j = 0; j < 1.0; j += pixel_size_sm) {
					if (insideTriangle(P.x + i, P.y + j, rec)) {
						count++;

						// 引入SSAA后要计算4个像素的点
						Vec3f recP = Vec3f(P.x + i , P.y + j , P.z);

						// 获取重心坐标 [alpha, beta, gama]
						Vec3f bc_screen = getBarycentric(screen_triangleV[0], screen_triangleV[1], screen_triangleV[2], recP);
						if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
							continue;
						}

						// 重心坐标插值求法向量
						// ERROR: 有问题！
						Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];
						//// 求片元平面的法向量 (old)
						//Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
						//n.normalize();

						// 重心坐标插值求颜色
						// 光照部分仅计算了漫反射 
						// Ld = kd * (I / r^2) * max(0, n * light_dir);
						float intensity = lightDir * vertexNormal;
						r += intensity * 255 / 4;
						g += intensity * 255 / 4;
						b += intensity * 255 / 4;
						
						// 重心坐标插值求z
						P.z = 0;
						float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;

						// z-buffer判定
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
				// 引入SSAA后要计算4个像素的点
				//Vec3f recP = Vec3f(P.x + i , P.y + j , P.z);

				// 获取重心坐标 [alpha, beta, gama]
				Vec3f bc_screen = getBarycentric(screen_triangleV[0], screen_triangleV[1], screen_triangleV[2], P);
				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) {
					continue;
				}

				// 重心坐标插值求法向量
				// ERROR: 有问题！
				Vec3f vertexNormal = vertex_normals[0] * bc_screen[0] + vertex_normals[1] * bc_screen[1] + vertex_normals[2] * bc_screen[2];

				// 重心坐标插值求颜色
				float intensity = lightDir * vertexNormal;
				r = intensity * 255 * count / 4;
				g = intensity * 255 * count / 4;
				b = intensity * 255 * count / 4;
				// SSAA / MSAA 求平均颜色
				Color color = Color(r, g, b, 1);

				// 重心坐标插值求

				// 重心坐标插值求z
				P.z = 0;
				float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;

				// z-buffer判定
				if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
					(*zBuffer)(P.x, P.y) = z_interpolated;
					(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
				}

				//// SSAA 版本 勿删
				//if ((float)z_interpolated > (*zBuffer_SSAA)(P.x + i, P.y + j)) {
				//	(*zBuffer_SSAA)(P.x + i, P.y + j) = z_interpolated;
				//	(*pixelBuffer_SSAA)(P.x + i, P.y + j) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
				//	(*pixelBuffer)(P.x, P.y) += (*pixelBuffer_SSAA)(P.x + i, P.y + j);
				//}
			}*/

			//Color color = Color(r, g, b, 1);
			//// 重心坐标插值求z
			//P.z = 0;
			//float z_interpolated = bc_screen[0] * screen_triangleV[0].z + bc_screen[1] * screen_triangleV[1].z + bc_screen[2] * screen_triangleV[2].z;
			//// z-buffer判定
			//if ((float)z_interpolated > (*zBuffer)(P.x, P.y)) {
			//	(*zBuffer)(P.x, P.y) = z_interpolated;
			//	(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			//}
			
			// 如果遍历到了边缘像素 则重置 深度缓存，防止黑边
			//if (count < maxCount) {
			//	(*zBuffer)(P.x, P.y) = std::numeric_limits<float>::min();
			//}
			if (flag) {
				// SSAA / MSAA 求平均颜色
				Color color = Color(r, g, b, 1);
				(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			}
		}
	}

}

// 当前使用
void Rasterization::triangle(Vec3f* triangleV, IShader &shader, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, const SDL_PixelFormat* MapFormat) {
	// 生成边界盒
	BoundingBox boundBox = BoundingBox(triangleV, DisplayHandler::screenWidth, DisplayHandler::screenHeight);

	Vec3f P;
	Color color;

	// 遍历边界盒 设置每个像素
#pragma omp parallel for 
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			
			// 获取重心坐标 [alpha, beta, gama]
			Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);
			P.z = 0;
			//bc_screen = bc_screen * (1 / (bc_screen.x + bc_screen.y + bc_screen.z));
			float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;
			
			// 跳过条件: 1.不在三角形内; 2.zbuffer检测未通过
			// NOTICE: 黑边问题已经解决，原因其实很简单，计算重心坐标时，由于精度丢失或者其他原因
			// 在边缘上的点的重心坐标值小于0（实际应该等于0），故而未能渲染到
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

			// 可见性检测
			bool visible = shader.fragment(bc_screen, color);
			if (visible) {
				(*zBuffer)(P.x, P.y) = z_interpolated;
				(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
			}
		}
	}
}

void Rasterization::triangle(Vec3f* triangleV, Buffer<float>* zBuffer, Buffer<Uint32>* pixelBuffer, Color color, const SDL_PixelFormat* MapFormat) {
	// 生成边界盒
	BoundingBox boundBox = BoundingBox(triangleV, DisplayHandler::screenWidth, DisplayHandler::screenHeight);

	Vec3f P;
	std::vector<Vec3f> rec = std::vector<Vec3f>{ triangleV[0], triangleV[1], triangleV[2] };

	// 遍历边界盒 设置每个像素
	for (P.x = boundBox.minX; P.x <= boundBox.maxX; P.x++) {
		for (P.y = boundBox.minY; P.y <= boundBox.maxY; P.y++) {
			// 获取重心坐标 [alpha, beta, gama]
			Vec3f bc_screen = getBarycentric(triangleV[0], triangleV[1], triangleV[2], P);
			float z_interpolated = bc_screen[0] * triangleV[0].z + bc_screen[1] * triangleV[1].z + bc_screen[2] * triangleV[2].z;

			// 跳过条件: 1.不在三角形内; 2.zbuffer检测未通过
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 || z_interpolated < (*zBuffer)(P.x, P.y)) {
				continue;
			}

			(*zBuffer)(P.x, P.y) = z_interpolated;
			(*pixelBuffer)(P.x, P.y) = SDL_MapRGB(MapFormat, color.r, color.g, color.b);
		}
	}
}

