#include "geometry.h"


Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) {

}

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            E[i][j] = (i == j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i >= 0 && i < rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                result.m[i][j] += m[i][k] * a.m[k][j];
            }
        }
    }
    return result;
}

// 矩阵的转置
Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[j][i] = m[i][j];
    return result;
}

// 矩阵的逆
Matrix Matrix::inverse() {
    assert(rows == cols);
    // augmenting the square matrix with the identity matrix of the same dimensions A => [AI]
    Matrix result(rows, cols * 2);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = m[i][j];
    for (int i = 0; i < rows; i++)
        result[i][i + cols] = 1;
    // first pass
    for (int i = 0; i < rows - 1; i++) {
        // normalize the first row
        for (int j = result.cols - 1; j >= 0; j--)
            result[i][j] /= result[i][i];
        for (int k = i + 1; k < rows; k++) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // normalize the last row
    for (int j = result.cols - 1; j >= rows - 1; j--)
        result[rows - 1][j] /= result[rows - 1][rows - 1];
    // second pass
    for (int i = rows - 1; i > 0; i--) {
        for (int k = i - 1; k >= 0; k--) {
            float coeff = result[k][i];
            for (int j = 0; j < result.cols; j++) {
                result[k][j] -= result[i][j] * coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            truncate[i][j] = result[i][j + cols];
    return truncate;
}

// 模型变换
Matrix Matrix::getZoom(float ratio) {
    Matrix m = Matrix::identity(4);
    m[0][0] = ratio;
    m[1][1] = ratio;
    m[2][2] = ratio;
    //m[3][3] = ratio;
    return m;
}

Matrix Matrix::getTranslate(float tx, float ty, float tz) {
    Matrix m = Matrix::identity(4);
    m[0][3] = tx;
    m[1][3] = ty;
    m[2][3] = tz;

    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
    return m;
}

Matrix Matrix::getRotationX(float anglex) {
    anglex *= (M_PI / 180.f);
    Matrix m = Matrix::identity(4);
    m[1][1] = std::cos(anglex);
    m[1][2] = -std::sin(anglex);
    m[2][1] = std::sin(anglex);
    m[2][2] = std::cos(anglex);
    return m;
}

Matrix Matrix::getRotationY(float angley) {
    angley *= (M_PI / 180.f);
    Matrix m = Matrix::identity(4);
    m[0][0] = std::cos(angley);
    m[0][2] = std::sin(angley);
    m[2][0] = -std::sin(angley);
    m[2][2] = std::cos(angley);
    return m;
}

Matrix Matrix::getRotationZ(float anglez) {
    anglez *= (M_PI / 180.f);
    Matrix m = Matrix::identity(4);
    m[0][0] = std::cos(anglez);
    m[0][1] = -std::sin(anglez);
    m[1][0] = std::sin(anglez);
    m[1][1] = std::cos(anglez);
    return m;
}

// 摄像机变换
Matrix Matrix::getCameraTrans() {
    Matrix camera = Matrix::identity(4);
    // TODO:
    return camera;
}

// 投影变换
Matrix Matrix::getPersProjection(float znear, float zfar, float aspectRatio, float eyeAngle) {
    Matrix Projection = Matrix::identity(4);

    float h = znear * std::tan(eyeAngle * M_PI / (2 * 180)) * 2;
    float w = h * aspectRatio;
    float z = zfar - znear;

    // 获取正交变换的矩阵 （旋转 * 平移）
    Matrix ortho = Matrix::identity(4);
    ortho[0][0] = 2 / w;
    ortho[1][1] = 2 / h;
    ortho[2][2] = 2 / z;
    ortho[3][3] = 1;

    ortho[2][3] = -(znear + zfar) / 2;

    // 获取正交到透视的变换矩阵（推导请参考101）
    Matrix perspToOrtho = Matrix::identity(4);
    perspToOrtho[0][0] = znear;
    perspToOrtho[1][1] = znear;
    perspToOrtho[2][2] = znear + zfar;

    perspToOrtho[2][3] = -znear * zfar;
    perspToOrtho[3][2] = 1;

    Projection = ortho * perspToOrtho * Projection;

    return Projection;
}

Matrix Matrix::getPersProjection2(float fov, float AR, float near, float far) {
    Matrix projectionMat;
    // 2.0版本 获取透视投影矩阵
    float tanHalfFOVInverse = 1 / tan((fov / 2) * (M_PI / 180));

    projectionMat[0][0] = tanHalfFOVInverse;
    projectionMat[1][1] = AR * tanHalfFOVInverse;
    projectionMat[2][2] = (near) / (far - near);
    projectionMat[2][3] = (far * near) / (far - near);

    projectionMat[3][2] = -1;

    return projectionMat;
}

// 获取视口变换矩阵
Matrix Matrix::getViewport(int x, int y, int w, int h) {
    // it seems not similar to games101 viewport trans matrix
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = 0;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = 1.f;
    return m;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i = 0; i < m.nrows(); i++) {
        for (int j = 0; j < m.ncols(); j++) {
            s << m[i][j];
            if (j < m.ncols() - 1) s << "\t";
        }
        s << "\n";
    }
    return s;
}


Matrix VecToMatrix(Vec3f vec) {
    Matrix m(4, 1);
    m[0][0] = vec.x;
    m[1][0] = vec.y;
    m[2][0] = vec.z;
    m[3][0] = 1.f;
    return m;
}

Vec3f MatrixToVec(Matrix m) {
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}