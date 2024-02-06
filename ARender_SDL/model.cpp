#include "Model.h"

Model::Model(const char* filename) : verts_(), facesVertex_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cout << "��ģ���ļ�ʧ��,ģ������:" << filename << std::endl;
        return;
    }

    std::string line;
    std::string key;                        // ���ݱ�ʶ

    char trash;

    std::string faceX, faceY, faceZ;        // ƬԪ��������

    while (!in.eof()) {
        std::getline(in, line);                     // ��\nΪ�ָ���,��ȡ obj �ļ���ÿ������
        std::istringstream iss(line.c_str());       // �Կո�Ϊ�ָ���
        iss >> key;
        if (key == "v") {
            // v: ÿ������
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }
            verts_.push_back(v);
            //std::cout << "vertex: " << v.x << "," << v.y << "," << v.z << "." << std::endl;
        }
        else if (key == "vt") {
            // vt: �������꣬��ֵΪu, v
            Vec3f vt;
            for (int i = 0; i < 3; i++) {
                iss >> vt.raw[i];
            }
            texels_.push_back(vt);
        }
        else if (key == "vn") {
            // vn: ���㷨��������ֵΪx,y,z �������Ǳ�ʾ����ĳ���
            Vec3f vn;
            for (int i = 0; i < 3; i++) {
                iss >> vn.raw[i];
            }
            normals_.push_back(vn);
        }
        else if (key == "f") {
            //f����ʾһ���棬������v/vt/vn��������ʽ��ɡ�
            // ����obj�ļ���f 5/15/7 4/14/6 6/16/8 ��
            // ��ʾ�ɵ�5����4����6���������������һ������ƽ��,
            // ƽ��������ɵ�15����14����16���������������γɣ�
            // ���ƽ��ĳ����ǵ�7����6����8����������ķ�������ƽ��ֵ��
            
            std::vector<int> faceV;
            std::vector<int> faceVN;
            std::vector<int> faceVT;

            iss >> faceX >> faceY >> faceZ;
            // ��ȡ obj �ļ��� f ���ݶ�
            std::vector<std::string> splitX = splitStr(faceX, '/');
            std::vector<std::string> splitY = splitStr(faceY, '/');
            std::vector<std::string> splitZ = splitStr(faceZ, '/');

            // ƽ��Ķ���
            // ��Ҫ -1 ��Ϊ���������0
            faceV.push_back(std::stoi(splitX[0]) - 1);
            faceV.push_back(std::stoi(splitY[0]) - 1);
            faceV.push_back(std::stoi(splitZ[0]) - 1);

            // ƽ���������ķ�����
            faceVN.push_back(std::stoi(splitX[2]) - 1);
            faceVN.push_back(std::stoi(splitY[2]) - 1);
            faceVN.push_back(std::stoi(splitZ[2]) - 1);

            // ƽ�����������
            faceVT.push_back(std::stoi(splitX[1]) - 1);
            faceVT.push_back(std::stoi(splitY[1]) - 1);
            faceVT.push_back(std::stoi(splitZ[1]) - 1);

            // ���뵽ģ�����ݼ�����
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

// ����ģ��ƬԪ����
std::vector<int> Model::face(int idx) {
    return facesVertex_[idx];
}

// ����ģ�Ͷ�������
Vec3f Model::vert(int i) {
    return verts_[i];
}

// �ָ��ַ���
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