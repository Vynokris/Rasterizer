#include <cstring>

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(int width, int height)
    : _width(width)
    , _height(height)
{
    // Create the framebuffer (color+depth+opengl texture).
    // We need an OpenGL texture to display the result of the renderer to the screen.

    // Load the color and depth buffers.
    colorBuffer.reserve(width * height);
    depthBuffer.reserve(width * height);

    // Load the texture.
    glGenTextures(1, &_colorTexture);
    glBindTexture(GL_TEXTURE_2D, _colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &_colorTexture);
}

void Framebuffer::clear()
{
    // Clear color buffer
    Color* colors = colorBuffer.data();

    // Fill the first line with the clear color
    for (std::size_t i = 0; i < (std::size_t)_width; ++i)
        std::memcpy(&colors[i], &clearColor, sizeof(Color));

    // Copy the first line onto every line
    for (std::size_t i = 1; i < (std::size_t)_height; ++i)
        std::memcpy(&colors[i * _width], &colors[0], _width * sizeof(Color));

    // Clear depth buffer
    std::memset(depthBuffer.data(), 0, depthBuffer.size() * sizeof(depthBuffer[0]));
}

void Framebuffer::updateTexture()
{
    // Reload the texture.
    glBindTexture(GL_TEXTURE_2D, _colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, colorBuffer.data());
}
