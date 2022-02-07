#pragma once

#include <Framebuffer.hpp>

// Vertex format (only one supported for now)
struct rdrVertex
{
    float x, y, z;    // Pos
    float nx, ny, nz; // Normal
    Color color;      // Color
    float u, v;       // Texture coordinates
};


struct Viewport
{
    Viewport(const int _x, const int _y, unsigned int _width, unsigned int _height)
        : x(_x), y(_y), width(_width), height(_height) {}

    int x, y;
    unsigned int width, height;
};


class Renderer
{
private:
    Viewport viewport;
    Color lineColor = { 1.f, 1.f, 1.f, 1.f };

    Mat4 modelMat;
    Mat4 viewMat;
    Mat4 projectionMat;

public:
    Framebuffer framebuffer;
    
    Renderer(const unsigned int& width, const unsigned int& height);
    ~Renderer();

    //TODO
    void setProjection(const Mat4& projectionMatrix);
    void setView(const Mat4& viewMatrix);
    void setModel(const Mat4& modelMatrix);
    void setViewport(const int x, const int y, const unsigned int width, const unsigned int height);
    void setTexture(float* colors32Bits, const unsigned int width, const unsigned int height);
    void drawPixel(unsigned int x, unsigned int y, Color color);
    void drawLine(Vector3 p0, Vector3 p1, const Color& color);
    void drawTriangles(rdrVertex* vertices, const unsigned int count);

private:
    void drawTriangle(rdrVertex* vertices);
    void showImGuiControls();
};


