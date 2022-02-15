#include "BmpLoader.hpp"

#include <cstdio>
using namespace arithmetic;

Color TextureData::getPixelColor(const int& x, const int& y)
{
    // Idk why but the first row of data is totally irrelevant (I use y+1).
    int index = (abs(y) * (width + padding) + abs(x)) * 3;
    return Color {
        pixels[index+2] / 255.f,
        pixels[index+1] / 255.f,
        pixels[index]   / 255.f,
        1
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

GLuint loadTextureFromData(const TextureData& _textureData, GLuint _texture, const bool& _deleteData)
{
    // Allocate a new texture and bind it to a 2D target.
    if (_texture == 0)
        glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    // Set some texture parameters.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);

    // Add the file data onto the texture and free the data.
    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, abs(_textureData.width), abs(_textureData.height), 0, GL_BGR, GL_UNSIGNED_BYTE, _textureData.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (_deleteData)
        delete[] _textureData.pixels;
    return _texture;
}

GLuint loadBmpTexture(const char* _filename)
{
    // Create the texture and the bmp header object.
    GLuint texture;
    BmpHeader bmpHeader;

    // Open the texture file.
    FILE* f;
    f = fopen(_filename, "rb");
    if (f == NULL) return 0;

    // Read the file metadata.
    fread(&bmpHeader, 54, 1, f);
    if (bmpHeader.fileType != 0x4D42 || bmpHeader.bitsPerPixel != 24)
        return 0;

    // Read the file data and close it.
    unsigned char* data = new unsigned char[bmpHeader.sizeOfBitmap];
    fseek(f, bmpHeader.dataOffset, 0);
    fread(data, bmpHeader.sizeOfBitmap, 1, f);
    fclose(f);

    // Allocate a new texture and bind it to a 2D target.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set some texture parameters.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);

    // Add the file data onto the texture and free the data.
    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, abs(bmpHeader.width), abs(bmpHeader.height), 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    return texture;
}

GLuint loadTestTexture()
{
    // Create the texture, its width and height, and its data.
    GLuint texture;
    int width  = 2;
    int height = 2;
    unsigned char* data = new unsigned char[width*height*3];

    // Write colors to the texture.
    data[0]  = 255; data[1]  = 0;   data[2]  = 0;
    data[3]  = 0;   data[4]  = 255; data[5]  = 0;
    data[6]  = 0;   data[7]  = 0;   data[8] = 255;
    data[9] = 255;  data[10] = 255; data[11] = 255;

    // Allocate a new texture and bind it to a 2D target.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set some texture parameters.
    glPixelStoref  (GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);

    // Add the file data onto the texture and free the data.
    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    return texture;
}
