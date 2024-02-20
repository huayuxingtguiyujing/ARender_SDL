#include "Model.h"

Model::Model(std::string modelName, std::string sceneName) : verts_(), facesVertex_() {

    // 获得模型的名称
    std::string modelPath = "scene/" + sceneName + "/obj/" + modelName;
    std::string texturePath = "scene/" + sceneName + "/texture";

    // 加载纹理
    diffuseTexture = TGAImage();
    TGAImage::load_texture(texturePath, getStrFileName(modelName) + "_diffuse.tga", diffuseTexture);
    //std::cout << "The texture condition: " << textureTest2.get_width() << ", " << textureTest2.get_height() << std::endl;

    // 打开模型文件
    std::ifstream in;
    in.open(modelPath, std::ifstream::in);
    if (in.fail()) {
        std::cout << "打开模型文件失败,模型名称:" << modelPath << std::endl;
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
    std::cerr << "vertex num: " << verts_.size() << ", face num: " << facesVertex_.size() 
        << "face vertexs" << facesVertex_.size() << "face uv: " << facesTexture_.size() << ", face normals: " << facesNormal_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)facesVertex_.size();
}



// 访问 顶点的法向量
Vec3f Model::normal(int index) {
    return normals_[index];
}

// 访问 顶点坐标
Vec3f Model::vert(int index) {
    return verts_[index];
}

// 访问 顶点法向量
Vec3f Model::texels(int index) {
    return texels_[index];
}

// 访问 片元三个顶点坐标的index
std::vector<int> Model::faceVertex(int index) {
    return facesVertex_[index];
}

// 访问 片元三个点的法向量的index
std::vector<int> Model::faceNormal(int index) {
    return facesNormal_[index];
}

// 访问 片元三个点uv的index
std::vector<int> Model::faceTexture(int index) {
    return facesTexture_[index];
}

// TODO: Texture 类需要修改，此方法无法正常使用
Texture Model::getTextureTest() {
    return textureTest;
}

Vec3f Model::getDiffuse(float u, float v) {
    return diffuseTexture.get(u, v, false);
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

// 分割文件名
std::string Model::getStrFileName(std::string filename) {
    std::string str = std::string(filename);
    size_t lastDotPos = str.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        str = str.substr(0, lastDotPos);
    }
    return str;
}