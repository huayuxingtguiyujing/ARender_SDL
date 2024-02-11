#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <cmath>

// TODO: 导入 stb_image 包
// TODO: 完成这部分
Texture::Texture(std::string path, std::string type) {
    //stbi_set_flip_vertically_on_load(true);

    //unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    const char* data = "s";
    widthInTiles = (width + tileW - 1) / tileW;
    pixelData = new float[width * height * channels];

    if (data) {
        if (type == "RGB") { 
            //Rgb data requires a gamma correction, float conversion
            for (int i = 0; i < width * height * channels; ++i) {
                pixelData[i] = std::pow((float)data[i] * (1 / 255.0f), 2.2f);
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
    //stbi_image_free(data);
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

                linearIndex = (tilePixelHeight * width + tileCol * tileW + tileRow * width * tileH) * channels;

                for (int tilePixelWidth = 0; tilePixelWidth < tileW; ++tilePixelWidth) {

                    //Pixel wise movement is equal to channelwise movement in the array

                    for (int pC = 0; pC < channels; ++pC) {

                        tiledPixelData[tiledIndex] = pixelData[linearIndex];
                        ++linearIndex;
                        ++tiledIndex;

                    }
                }
            }
        }
    }
    delete[] pixelData;
    pixelData = tiledPixelData;
}

Texture::~Texture() {
    delete[] pixelData;
}

// TODO: 这个方法是干嘛的？
Vec3f Texture::getPixelVal(float u, float v) {

    int uInt = u * (width - 1);
    int vInt = v * (height - 1);

    int tileX = uInt / tileW;
    int tileY = vInt / tileH;

    int inTileX = uInt % tileW;
    int inTileY = vInt % tileH;

    int index = ((tileY * widthInTiles + tileX) * (tileW * tileH)
        + inTileY * tileW + inTileX) * channels;

    return Vec3f{ pixelData[index], pixelData[index + 1], pixelData[index + 2] };

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

