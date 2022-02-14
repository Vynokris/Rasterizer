#pragma once

#include <vector>
#include <glad/gl.h>
#include <my_math.hpp>

class Framebuffer
{
private:
    // Framebuffer size.
    int width = 0, height = 0;

    // OpenGL texture (in VRAM)
    GLuint colorTexture = 0;

public:
    // In-RAM buffers.
    std::vector<Color> colorBuffer;
    std::vector<float> depthBuffer;

    // Default color for the framebuffer.
    Color clearColor = { 0.f, 0.f, 0.f, 1.f };

    // Constructor/Destructor.
    Framebuffer(const int& _width, const int& _height);
    ~Framebuffer();

    // Fill the framebuffer with the clear color.
    void clear(const int& zFar);

    // Update the opengl texture with the color buffer.
    void updateTexture();

    // Getters.
    int    getWidth()        const { return width; }
    int    getHeight()       const { return height; }
    GLuint getColorTexture() const { return colorTexture; }
};