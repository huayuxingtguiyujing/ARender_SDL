#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>

Texture::Texture() {
}

// TODO: 导入 stb_image 包
// TODO: 完成这部分
Texture::Texture(std::string path, std::string type) {
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    std::cout << "该图片的数据: " << static_cast<unsigned int>(data[0]) << ","
     << static_cast<unsigned int>(data[1]) << ","
     << static_cast<unsigned int>(data[2]) << ","
     << static_cast<unsigned int>(data[3]) << "," << channels << std::endl;

    widthInTiles = (width + tileW - 1) / tileW;
    pixelData = new float[width * height * channels];

    if (data) {
        // 一律按照 RGB 格式处理
        for (int i = 0; i < width * height * channels; ++i) {
            pixelData[i] = std::pow((float)data[i] * (1 / 255.0f), 2.2f);
            pixelData[i] = static_cast<unsigned int>(data[i]);
            //std::cout << pixelData[i] << std::endl;
        }
        
    }
    stbi_image_free(data);
    return;

    if (data) {
        if (type == "RGB") { 
            //Rgb data requires a gamma correction, float conversion
            for (int i = 0; i < width * height * channels; ++i) {
                pixelData[i] = std::pow((float)data[i] * (1 / 255.0f), 2.2f);
                //pixelData[i] = data[i];
                //std::cout << pixelData[i] << std::endl;
            }
            tileData();
        }
        else if (type == "XYZ") {
            //conversion to float and rescaling to -1 1 bounds
            for (int i = 0; i < width * height * channels; ++i) {
                pixelData[i] = (float)data[i] * (2 / 255.0f) - 1.0f;
            }
            tileData();
        }
        else if (type == "BW") { 
            //Simple conversion to float
            for (int i = 0; i < width * height * channels; ++i) {
                pixelData[i] = (float)data[i] * (1 / 255.0f);
            }
            tileData();
        }
        else {
            printf("Error unrecognized texture format type.\n");
        }
    }
    else {
        printf("Failed to load texture at: %s\n", path.c_str());
    }
    stbi_image_free(data);

    std::cout << "successfully load texture: " << "" << std::endl;

}

// TODO: 这个方法是干嘛的？
void Texture::tileData() {
    float* tiledPixelData = new float[width * height * channels];

    int tileNumW = width / tileW;
    int tileNumH = height / tileH;
    int linearIndex = 0;
    int tiledIndex = 0;

    for (int tileRow = 0; tileRow < tileNumW; ++tileRow) {
        for (int tileCol = 0; tileCol < tileNumH; ++tileCol) {

            for (int tilePixelHeight = 0; tilePixelHeight < tileH; ++tilePixelHeight) {
                // 
                linearIndex = (tilePixelHeight * width + tileCol * tileW + tileRow * width * tileH) * channels;
                for (int tilePixelWidth = 0; tilePixelWidth < tileW; ++tilePixelWidth) {

                    //Pixel wise movement is equal to channelwise movement in the array

                    for (int pC = 0; pC < channels; ++pC) {

                        tiledPixelData[tiledIndex] = pixelData[linearIndex];
                        //std::cout << "Have set: " << tiledPixelData[tiledIndex] << std::endl;
                        ++linearIndex;
                        ++tiledIndex;

                    }
                }
            }
        }
    }
    delete[] pixelData;
    pixelData = tiledPixelData;
    //std::cout << "Have set: " << pixelData[0] << std::endl;
    //std::cout << "Have set: " << pixelData[1] << std::endl;
    //std::cout << "Have set: " << pixelData[2] << std::endl;
}

Texture::~Texture() {
    //delete[] pixelData;
}

// TODO: 这个方法是干嘛的？
Vec3f Texture::getPixelVal(float u, float v) {

    //std::cout << "the pixel data: " << pixelData[0] << std::endl;

    int uInt = u * (width - 1);
    int vInt = v * (height - 1);
    int pixelIndex = uInt + vInt * (width - 1);
    

    //int tileX = uInt / tileW;
    //int tileY = vInt / tileH;

    //int inTileX = uInt % tileW;
    //int inTileY = vInt % tileH;

    //int index = ((tileY * widthInTiles + tileX) * (tileW * tileH)
    //    + inTileY * tileW + inTileX) * channels;

    //std::cout << "现在输出pixel颜色: " << u << ", " << v << ", index为: " << index << std::endl;
    //std::cout << "输出pixelData的值: " << pixelData[pixelIndex] << "," << pixelData[pixelIndex + 1] << "," << pixelData[pixelIndex + 2] << std::endl;

    return Vec3f{ pixelData[pixelIndex], pixelData[pixelIndex + 1], pixelData[pixelIndex + 2] };

}

// TODO: 这个方法是干嘛的？
float Texture::getIntensityVal(float u, float v) {
    
    int uInt = u * (width - 1);
    int vInt = v * (height - 1);

    int tileX = uInt / tileW;
    int tileY = vInt / tileH;

    int inTileX = uInt % tileW;
    int inTileY = vInt % tileH;

    int index = ((tileY * widthInTiles + tileX) * (tileW * tileH)
        + inTileY * tileW
        + inTileX);

    return pixelData[index];
}

