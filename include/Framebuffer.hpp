#pragma once

#include <vector>
#include "common.hpp"
#include <glad/gl.h>

class Framebuffer
{
private:
    // Framebuffer size.
    int _width = 0, _height = 0;

    // OpenGL texture (in VRAM)
    GLuint _colorTexture = 0;

public:
    // In-RAM buffers.
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;

    // Default color for the framebuffer.
    Color clearColor = { 0.f, 0.f, 0.f, 1.f };

    // Constructor/Destructor.
    Framebuffer(int width, int height);
    ~Framebuffer();

    // Fill the framebuffer with the clear color.
    void clear();

    // Update the opengl texture with the color buffer.
    void updateTexture();

    // Getters.
    int    getWidth()        const { return _width; }
    int    getHeight()       const { return _height; }
    GLuint getColorTexture() const { return _colorTexture; }
};