#include "Model.h"

Model::Model(const char* filename) : verts_(), facesVertex_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cout << "打开模型文件失败,模型名称:" << filename << std::endl;
        return;
    }

    std::string line;
    std::string key;                        // 数据标识

    char trash;

    std::string faceX, faceY, faceZ;        // 片元的三顶点

    while (!in.eof()) {
        std::getline(in, line);                     // 以\n为分隔符,读取 obj 文件的每行数据
        std::istringstream iss(line.c_str());       // 以空格为分隔符
        iss >> key;
        if (key == "v") {
            // v: 每个顶点
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }
            verts_.push_back(v);
            //std::cout << "vertex: " << v.x << "," << v.y << "," << v.z << "." << std::endl;
        }
        else if (key == "vt") {
            // vt: 纹理坐标，其值为u, v
            Vec3f vt;
            for (int i = 0; i < 3; i++) {
                iss >> vt.raw[i];
            }
            texels_.push_back(vt);
        }
        else if (key == "vn") {
            // vn: 顶点法向量，其值为x,y,z 法向量是表示顶点的朝向
            Vec3f vn;
            for (int i = 0; i < 3; i++) {
                iss >> vn.raw[i];
            }
            normals_.push_back(vn);
        }
        else if (key == "f") {
            //f：表示一个面，由三个v/vt/vn的索引形式组成。
            // 比如obj文件中f 5/15/7 4/14/6 6/16/8 ，
            // 表示由第5、第4、第6这三个顶点组成了一个三角平面,
            // 平面的纹理由第15、第14、第16这三个纹理坐标形成，
            // 这个平面的朝向是第7、第6、第8这三个顶点的法向量求平均值。
            
            std::vector<int> faceV;
            std::vector<int> faceVN;
            std::vector<int> faceVT;

            iss >> faceX >> faceY >> faceZ;
            // 读取 obj 文件的 f 数据段
            std::vector<std::string> splitX = splitStr(faceX, '/');
            std::vector<std::string> splitY = splitStr(faceY, '/');
            std::vector<std::string> splitZ = splitStr(faceZ, '/');

            // 平面的顶点
            // 需要 -1 因为顶点起点是0
            faceV.push_back(std::stoi(splitX[0]) - 1);
            faceV.push_back(std::stoi(splitY[0]) - 1);
            faceV.push_back(std::stoi(splitZ[0]) - 1);

            // 平面各个顶点的法向量
            faceVN.push_back(std::stoi(splitX[2]) - 1);
            faceVN.push_back(std::stoi(splitY[2]) - 1);
            faceVN.push_back(std::stoi(splitZ[2]) - 1);

            // 平面的纹理坐标
            faceVT.push_back(std::stoi(splitX[1]) - 1);
            faceVT.push_back(std::stoi(splitY[1]) - 1);
            faceVT.push_back(std::stoi(splitZ[1]) - 1);

            // 加入到模型数据集合中
            facesVertex_.push_back(faceV);
            facesNormal_.push_back(faceVN);
            facesTexture_.push_back(faceVT);
        }
        
    }
    std::cerr << "vertex num: " << verts_.size() << ", face num: " << facesVertex_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)facesVertex_.size();
}

// 访问模型片元数据
std::vector<int> Model::face(int idx) {
    return facesVertex_[idx];
}

// 访问模型顶点数据
Vec3f Model::vert(int i) {
    return verts_[i];
}

// 分割字符串
std::vector<std::string> Model::splitStr(std::string& str, char deli) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> splitString;
    while (std::getline(ss, token, deli)) {
        if (token == " ") {
            splitString.push_back("0");
        }
        else {
            splitString.push_back(token);
        }
    }
    return splitString;
}