#include <cstring>

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(const int& _width, const int& _height)
    : width(_width)
    , height(_height)
{
    // Create the framebuffer (color+depth+opengl texture).
    // We need an OpenGL texture to display the result of the renderer to the screen.

    // Load the color and depth buffers.
    colorBuffer.reserve(_width * _height);
    depthBuffer.reserve(_width * _height);

    // Load the texture.
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &colorTexture);
}

void Framebuffer::clear(const int& zFar)
{
    // Clear color and depth buffer
    colorBuffer.assign(width * height, clearColor);
    depthBuffer.assign(width * height, zFar);
}

void Framebuffer::updateTexture()
{
    // Reload the texture.
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, colorBuffer.data());
}
