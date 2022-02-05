#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>

#include "Renderer.hpp"

Renderer::Renderer(const unsigned int& width, const unsigned int& height)
    : viewport(0, 0, width, height)
    , framebuffer(width, height)
{
}

Renderer::~Renderer()
{
}

void Renderer::setProjection(const Mat4& projectionMatrix)
{
    projectionMat = projectionMatrix;
}

void Renderer::setView(const Mat4& viewMatrix)
{
    viewMat = viewMatrix;
}

void Renderer::setModel(const Mat4& modelMatrix)
{
    modelMat = modelMatrix;
}

void Renderer::setViewport(const int x, const int y, const unsigned int width, const unsigned int height)
{
    // TODO
}

void Renderer::setTexture(float* colors32Bits, const unsigned int width, const unsigned int height)
{
    // TODO
}

void Renderer::drawPixel(unsigned int x, unsigned int y, Color color)
{
    framebuffer.colorBuffer[y * framebuffer.getWidth() + x] = color;
}

void Renderer::drawLine(Vector3 p0, Vector3 p1, const Color& color)
{
    // Optimized Bresenham algorithm.
    /*
    // Get the direction between the points.
    int xDir = 1, yDir = 1;
    if (p0.x > p1.x) xDir = -1;
    if (p0.y > p1.y) yDir = -1;

    // Get the slope coefficient and error margin.
    int coeff       = 2     * (p1.y >= p0.y ? p1.y - p0.y : p0.y - p1.y);
    int slope_error = coeff - (p1.x >= p0.x ? p1.x - p0.x : p0.x - p1.x);
    
    // Trace line.
    for (int x = p0.x, y = p0.y; (xDir > 0 ? x <= p1.x : x >= p1.x); x += xDir)
    {
        // Draw current pixel.
        drawPixel(x, y, color);
        
        // Add slope to increment angle formed.
        slope_error += coeff;

        // Slope error reached 0, increment y and update slope error.
        if (slope_error >= 0)
        {
            y += yDir;
            slope_error -= 2 * (p1.x >= p0.x ? p1.x - p0.x : p0.x - p1.x);
        }
    }
    */

    // Filipe Del Pedro algorithm.
    int dx =  abs(p1.x - p0.x);
    int dy = -abs(p1.y - p0.y);
    int sx = p0.x < p1.x ? 1 : -1;
    int sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (true)
    {
        drawPixel(p0.x, p0.y, { 1, 0, 0, 1 });
        drawPixel(p1.x, p1.y, { 0, 1, 0, 1 });
        e2 = 2 * err;
        if (e2 >= dy)
        {
            if (p0.x == p1.x) break;
            err += dy; p0.x += sx;
        }
        if (e2 <= dx)
        {
            if (p0.y == p1.y) break;
            err += dx; p0.y += sy;
        }
    }
}

Vector3 ndcToScreenCoords(Vector3 ndc, const Viewport& viewport)
{
    // TODO
    return ndc;
}

void Renderer::drawTriangle(rdrVertex* vertices)
{
    // Store triangle vertices positions
    Vector3 localCoords[3] = {
        { vertices[0].x, vertices[0].y, vertices[0].z },
        { vertices[1].x, vertices[1].y, vertices[1].z },
        { vertices[2].x, vertices[2].y, vertices[2].z },
    };

    /*
    // Local space (v3) -> Clip space (v4)
    // TODO
    Vector4 clipCoords[3] = {
        { Vector4{ localCoords[0], 1.f } },
        { Vector4{ localCoords[1], 1.f } },
        { Vector4{ localCoords[2], 1.f } },
    };

    // Clip space (v4) to NDC (v3)
    // TODO
    Vector3 ndcCoords[3] = {
        { clipCoords[0].toVector3() },
        { clipCoords[1].toVector3() },
        { clipCoords[2].toVector3() },
    };

    // NDC (v3) to screen coords (v2)
    // TODO
    Vector3 screenCoords[3] = {
        { ndcToScreenCoords(ndcCoords[0], viewport) },
        { ndcToScreenCoords(ndcCoords[1], viewport) },
        { ndcToScreenCoords(ndcCoords[2], viewport) },
    };
    */

    Vector3 screenCoords[3] = {
        { vertices[0].x * 100 + 100, vertices[0].y * 100 + 100, vertices[0].z * 100 + 100 },
        { vertices[1].x * 100 + 100, vertices[1].y * 100 + 100, vertices[1].z * 100 + 100 },
        { vertices[2].x * 100 + 100, vertices[2].y * 100 + 100, vertices[2].z * 100 + 100 },
    };

    // Draw triangle wireframe
    drawLine(screenCoords[0], screenCoords[1], lineColor/*{ 1, 0, 0, 1 }*/);
    drawLine(screenCoords[1], screenCoords[2], lineColor/*{ 0, 1, 0, 1 }*/);
    drawLine(screenCoords[2], screenCoords[0], lineColor/*{ 0, 0, 1, 1 }*/);
}

void Renderer::drawTriangles(rdrVertex* vertices, const unsigned int count)
{
    // calculate mvp from matrices
    // Transform vertex list to triangles into colorBuffer
    for (int i = 0; i < (int)count; i += 3)
    {
        drawTriangle(&vertices[i]);
    }
}

void Renderer::showImGuiControls()
{
    ImGui::ColorEdit4("lineColor", lineColor.val);
}
