#pragma once

#include <vector>
#include "common.hpp"
#include <glad/gl.h>

class Framebuffer
{
private:
    int width = 0;
    int height = 0;

    // In-RAM buffers
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;

    // OpenGL texture (in VRAM)
    GLuint colorTexture = 0;

public:
    Color clearColor = { 0.f, 0.f, 0.f, 1.f };

    Framebuffer(int width, int height);
    ~Framebuffer();

    void Clear();
    void UpdateTexture();

    float* GetColorBuffer() { return reinterpret_cast<float*>(colorBuffer.data()); }
    float* GetDepthBuffer() { return depthBuffer.data(); }
    int GetWidth()  const   { return width; }
    int GetHeight() const   { return height; }

    GLuint GetColorTexture() const { return colorTexture; }



};