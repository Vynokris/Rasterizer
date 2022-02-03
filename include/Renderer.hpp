#pragma once

#include <Framebuffer.hpp>

// Vertex format (only one supported for now)
struct rdrVertex
{
    float x, y, z;    // Pos
    float nx, ny, nz; // Normal
    float r, g, b, a; // Color
    float u, v;       // Texture coordinates
};


struct Viewport
{

    Viewport(const int p_x, const int p_y, unsigned int p_width, unsigned int p_height):
    x(p_x),y(p_y),width(p_width),height(p_height){}

    const int x;
    const int y;
    unsigned int width;
    unsigned int height;
};


class Renderer
{
private:
    Framebuffer fb;
    Viewport viewport;

    Color lineColor = { 1.f, 1.f, 1.f, 1.f };

public:
// Color and depth buffer have to be valid until the shutdown of the renderer
// Color buffer is RGBA, each component is a 32 bits float
// Depth buffer is a buffer of 32bits floats
    Renderer(float* p_colorBuffer32Bits, float* p_depthBuffer, unsigned int p_width, unsigned int p_height);
    ~Renderer();

    //TODO
    void SetProjection(const Mat4& p_projectionMatrix);
    void SetView(const Mat4& p_viewMatrix);
    void SetModel(const Mat4& p_modelMatrix);
    void SetViewport(const int p_x, const int p_y, const unsigned int p_width, const unsigned int p_height);
    void SetTexture(float* p_colors32Bits, const unsigned int p_width, const unsigned int p_height);
    void DrawLine(const Vector3& p_0, const Vector3& p_1, const Color& p_color);
    void DrawTriangles(rdrVertex* p_vertices, const unsigned int p_count);


private:
    void DrawTriangle(rdrVertex* p_vertices);
    void ShowImGuiControls();
};


