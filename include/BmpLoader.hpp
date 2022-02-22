#pragma once

#include <glad/gl.h>
#include <cstdint>
#include <my_math.hpp>

#pragma pack(push, 1)
struct BmpHeader
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t sizeOfBitmap;
    int32_t  horizontalResolution;
    int32_t  verticalResolution;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

class TextureData
{
public:
    unsigned char* pixels;
    bool applyVertexColor = false;
    int width, height, padding;

    TextureData() : pixels(0), width(0), height(0), padding(0) {}
    ~TextureData() {  }

    Color getPixelColor(const int& _x, const int& _y, const float& alpha = 1);
};

TextureData loadBmpData(const char* _filename);

GLuint loadTextureFromData(const TextureData& _textureData, GLuint _texture = 0, const bool& _deleteData = false);

GLuint loadBmpTexture(const char* _filename);

GLuint loadTestTexture();
