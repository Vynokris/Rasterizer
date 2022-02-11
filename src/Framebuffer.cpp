#include <cstring>

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(const int& width, const int& height, const int& depth)
    : _width(width)
    , _height(height)
    , _depth(depth)
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
    // Clear color and depth buffer
    colorBuffer.assign(_width * _height, clearColor);
    depthBuffer.assign(_width * _height, _depth);
}

void Framebuffer::updateTexture()
{
    // Reload the texture.
    glBindTexture(GL_TEXTURE_2D, _colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _width, _height, 0, GL_RGBA, GL_FLOAT, colorBuffer.data());
}
