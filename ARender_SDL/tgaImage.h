#pragma once

#include <fstream>
#include <iostream>
#include "color.h"
#include "geometry.h"

#pragma pack(push,1)
struct TGA_Header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

class TGAImage {
protected:
    unsigned char* data;
    int width;
    int height;
    int bytespp;

    bool   load_rle_data(std::ifstream& in);
    bool unload_rle_data(std::ofstream& out);
public:
    enum Format {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage& img);
    ~TGAImage();

    bool read_tga_file(const char* filename);
    bool write_tga_file(const char* filename, bool rle = true);

    // ¼ÓÔØÎÆÀí
    static void load_texture(std::string filename, const char* suffix, TGAImage& img);

    // ÏñËØ²Ù×÷
    Color get(int x, int y);
    Vec3f get(float u, float v, bool flag);
    bool set(int x, int y, Color& c);
    bool set(int x, int y, const Color& c);

    // ÕÛµþ
    bool flip_horizontally();
    bool flip_vertically();

    bool scale(int w, int h);
   
    // get ²Ù×÷
    int get_width();
    int get_height();
    int get_bytespp();
    unsigned char* buffer();

    TGAImage& operator =(const TGAImage& img);

    void clear();
};
