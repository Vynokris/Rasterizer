#include <Texture.hpp>

#include <cstdio>
using namespace arithmetic;
#include <imgui.h>

Color TextureData::getPixelColor(const int& x, const int& y, const float& _alpha)
{
    // Idk why sometimes x and y go to -infinity so I just obliterate the pixel.
    if (x <= -2147483648 || y <= -2147483648) return { 0, 0, 0, 0 };

    // Find the index at which the pixel is located.
    int index = (y * (width + padding) + x) * 3;

    // Return the pixel's color (BGR -> RGB).
    return {
        pixels[index+2] / 255.f,
        pixels[index+1] / 255.f,
        pixels[index]   / 255.f,
        _alpha
    };
}

TextureData loadBmpData(const char* _filename)
{
    // Create the texture and the bmp header object.
    GLuint texture;
    BmpHeader bmpHeader;

    // Open the texture file.
    FILE* f;
    f = fopen(_filename, "rb");
    if (f == NULL) return TextureData();

    // Read the file metadata.
    fread(&bmpHeader, 54, 1, f);
    if (bmpHeader.fileType != 0x4D42 || bmpHeader.bitsPerPixel != 24)
        return TextureData();

    // Read the file data and close it.
    unsigned char* data = new unsigned char[bmpHeader.sizeOfBitmap];
    fseek(f, bmpHeader.dataOffset, 0);
    fread(data, bmpHeader.sizeOfBitmap, 1, f);
    fclose(f);

    // Create the texture data.
    TextureData textureData;
    textureData.pixels = data;

    // Set the width and height getters to the texture width and height.
    textureData.width  = bmpHeader.width;
    textureData.height = bmpHeader.height;

    // Find the offset at the end of each pixel line.
    if (!isPowerOf2(bmpHeader.width))
        textureData.padding = bmpHeader.width - getPowerOf2Under(bmpHeader.width);

    return textureData;
}
