#pragma once

#include <string>

#include "geometry.h"

class Texture {
public:
    Texture(std::string path, std::string type);
    ~Texture();

    Vec3f getPixelVal(float u, float v);
    float getIntensityVal(float u, float v);

private:
    float* pixelData;
    int width, height, channels, tileW = 32, tileH = 32, widthInTiles;

    //Currently disabled after tiling has been implemented
    int bilinearFiltering(float u, float v);

    //Reorganizes pixel data into a more cache friendly form
    void tileData();
};
