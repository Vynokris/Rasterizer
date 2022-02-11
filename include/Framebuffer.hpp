#pragma once

#include <vector>
#include <glad/gl.h>
#include <my_math.hpp>

class Framebuffer
{
private:
    // Framebuffer size.
    int _width = 0, _height = 0, _depth = 0;

    // OpenGL texture (in VRAM)
    GLuint _colorTexture = 0;

public:
    // In-RAM buffers.
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;

    // Default color for the framebuffer.
    Color clearColor = { 0.f, 0.f, 0.f, 1.f };

    // Constructor/Destructor.
    Framebuffer(const int& width, const int& height, const int& depth);
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