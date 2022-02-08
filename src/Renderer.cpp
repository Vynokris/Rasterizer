#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>
using namespace arithmetic;
using namespace render3D;

#include "Renderer.hpp"

Renderer::Renderer(const unsigned int& width, const unsigned int& height)
    : viewport(0, 0, width, height)
    , framebuffer(width, height)
{
    // Create renderer framebuffer (color+depth+opengl texture).
    // We need an OpenGL texture to display the result of the renderer to the screen.
}

Renderer::~Renderer()
{
}

void Renderer::setModel(const Mat4& modelMatrix)
{
    modelMat = modelMatrix;
}

void Renderer::setView(const Mat4& viewMatrix)
{
    viewMat = viewMatrix;
}

void Renderer::setProjection(const Mat4& projectionMatrix)
{
    projectionMat = projectionMatrix;
}

void Renderer::modelTranslate(float x, float y, float z)
{
    modelMat = modelMat * getTranslationMatrix({ x, y, z });
}

void Renderer::modelRotateX  (float angle)
{
    modelMat = modelMat * getXRotationMatrix(angle);
}

void Renderer::modelRotateY  (float angle)
{
    modelMat = modelMat * getYRotationMatrix(angle);
}

void Renderer::modelRotateZ  (float angle)
{
    modelMat = modelMat * getZRotationMatrix(angle);
}

void Renderer::modelScale    (float scaleX, float scaleY, float scaleZ)
{
    modelMat = modelMat * getScaleMatrix({ scaleX, scaleY, scaleZ });
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
    // Get the distance between the two points
    int dx =  abs(p1.x - p0.x);
    int dy = -abs(p1.y - p0.y);

    // Calculate the slope increments along the x and y axes.
    int sx = p0.x < p1.x ? 1 : -1;
    int sy = p0.y < p1.y ? 1 : -1;

    // Compute the error margins.
    int err = dx + dy;
    int e2;

    while (true)
    {
        //? This probably will be avoidable one we get the clip space right.
        if (0 <= p0.x && p0.x < viewport.width && 0 <= p0.y && p0.y < viewport.height)
            drawPixel(p0.x, p0.y, { 1, 0, 0, 1 });
        else break;

        e2 = 2 * err;
        if (e2 >= dy)
        {
            // Stop if the first point reached the second one.
            if (p0.x == p1.x) break;

            // Increment the error and the first point's x coordinate.
            err += dy; p0.x += sx;
        }
        if (e2 <= dx)
        {
            // Stop if the first point reached the second one.
            if (p0.y == p1.y) break;

            // Increment the error and the first point's y coordinate.
            err += dx; p0.y += sy;
        }
    }
}

static Vector3 ndcToScreenCoords(Vector3 ndc, const Viewport& viewport)
{
    return { ndc.x * viewport.width + viewport.width / 2, 
             ndc.y * viewport.height + viewport.height / 2, 
             ndc.z };
}

static int barycentricCoords(const Vector3& a, const Vector3& b, const Vector3& c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

void Renderer::drawTriangle(rdrVertex* vertices)
{
    // Store triangle vertices positions.
    Vector3 localCoords[3] = {
        { vertices[0].x, vertices[0].y, vertices[0].z },
        { vertices[1].x, vertices[1].y, vertices[1].z },
        { vertices[2].x, vertices[2].y, vertices[2].z },
    };
    
    // Local space (3D) -> World space (3D).
    Vector4 worldCoords[3] = {
        { (Vector4{ localCoords[0], 1 } * modelMat) },
        { (Vector4{ localCoords[1], 1 } * modelMat) },
        { (Vector4{ localCoords[2], 1 } * modelMat) },
    };
    
    // World space (3D) -> View space (3D).
    Vector4 viewCoords[3] = {
        { (worldCoords[0] * viewMat) },
        { (worldCoords[1] * viewMat) },
        { (worldCoords[2] * viewMat) },
    };
    
    // View space (3D) -> Clip space (4D).
    Vector4 clipCoords[3] = {
        { viewCoords[0] * projectionMat },
        { viewCoords[1] * projectionMat },
        { viewCoords[2] * projectionMat },
    };
    
    // Clip space (4D) -> NDC (3D).
    Vector3 ndcCoords[3] = {
        { clipCoords[0].getHomogenized().toVector3().getNormalized() },
        { clipCoords[1].getHomogenized().toVector3().getNormalized() },
        { clipCoords[2].getHomogenized().toVector3().getNormalized() },
    };
    
    // NDC (3D) -> screen coords (2D).
    Vector3 screenCoords[3] = {
        { ndcToScreenCoords(ndcCoords[0], viewport) },
        { ndcToScreenCoords(ndcCoords[1], viewport) },
        { ndcToScreenCoords(ndcCoords[2], viewport) },
    };

    #include <sstream>
    ImGui::Begin("Debug info");
    std::ostringstream temp;
    temp.precision(2);
    for (int i = 0; i < 3; i++) temp << "Local coords "  << i << ": (" << localCoords[i].x  << ", " << localCoords[i].y  << ", " << localCoords[i].z  << (i == 2 ? ")\n\n" : ")\n");
    for (int i = 0; i < 3; i++) temp << "World coords "  << i << ": (" << worldCoords[i].x  << ", " << worldCoords[i].y  << ", " << worldCoords[i].z  << ", " << worldCoords[i].w << (i == 2 ? ")\n\n" : ")\n");
    for (int i = 0; i < 3; i++) temp << "View coords "   << i << ": (" << viewCoords [i].x  << ", " << viewCoords [i].y  << ", " << viewCoords [i].z  << ", " << viewCoords [i].w << (i == 2 ? ")\n\n" : ")\n");
    for (int i = 0; i < 3; i++) temp << "Clip coords "   << i << ": (" << clipCoords [i].x  << ", " << clipCoords [i].y  << ", " << clipCoords [i].z  << ", " << clipCoords [i].w << (i == 2 ? ")\n\n" : ")\n");
    for (int i = 0; i < 3; i++) temp << "NDC coords "    << i << ": (" << ndcCoords  [i].x  << ", " << ndcCoords  [i].y  << ", " << ndcCoords  [i].z  << (i == 2 ? ")\n\n" : ")\n");
    temp.precision(4);
    for (int i = 0; i < 3; i++) temp << "Screen coords " << i << ": (" << screenCoords[i].x << ", " << screenCoords[i].y << ", " << screenCoords[i].z << (i == 2 ? ")\n\n" : ")\n");
    ImGui::Text(temp.str().c_str());
    ImGui::End();

    if (viewCoords[0].z >= 0 && viewCoords[1].z >= 0 && viewCoords[2].z >= 0)
        return;

    // Draw triangle wireframe
    // drawLine(screenCoords[0], screenCoords[1], lineColor);
    // drawLine(screenCoords[1], screenCoords[2], lineColor);
    // drawLine(screenCoords[2], screenCoords[0], lineColor);

    // Get the triangle's sides and normals.
    Vector2 p0p1(Vector2{ screenCoords[0].x, screenCoords[0].y }, Vector2{ screenCoords[1].x, screenCoords[1].y });
    Vector2 p1p2(Vector2{ screenCoords[1].x, screenCoords[1].y }, Vector2{ screenCoords[2].x, screenCoords[2].y });
    Vector2 p2p0(Vector2{ screenCoords[2].x, screenCoords[2].y }, Vector2{ screenCoords[0].x, screenCoords[0].y });
    float angle0 = p0p1.getAngleWithVector2(p1p2.getNormal());
    float angle1 = p1p2.getAngleWithVector2(p2p0.getNormal());
    float angle2 = p2p0.getAngleWithVector2(p0p1.getNormal());

    // Check if the triangle is inside out.
    if (angle0 < PI/2)
    {
        Vector3 tempVec = screenCoords[0];
        screenCoords[0] = screenCoords[1];
        screenCoords[1] = tempVec;
    }
    else if (angle1 < PI/2)
    {
        Vector3 tempVec = screenCoords[1];
        screenCoords[1] = screenCoords[2];
        screenCoords[2] = tempVec;
    }
    else if (angle2 < PI/2)
    {
        Vector3 tempVec = screenCoords[2];
        screenCoords[2] = screenCoords[0];
        screenCoords[0] = tempVec;
    }

    //* https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
    //* https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/

    // Compute triangle bounding box.
    int minX = min(min(screenCoords[0].x, screenCoords[1].x), screenCoords[2].x);
    int minY = min(min(screenCoords[0].y, screenCoords[1].y), screenCoords[2].y);
    int maxX = max(max(screenCoords[0].x, screenCoords[1].x), screenCoords[2].x);
    int maxY = max(max(screenCoords[0].y, screenCoords[1].y), screenCoords[2].y);

    // Clip against screen bounds.
    minX = clamp(minX, 0, (int)viewport.width  - 1);
    minY = clamp(minY, 0, (int)viewport.height - 1);
    maxX = clamp(maxX, 0, (int)viewport.width  - 1);
    maxY = clamp(maxY, 0, (int)viewport.height - 1);

    /*
    printf("\nCoords 0: %.1f, %.1f, %.1f\nCoords 1: %.1f, %.1f, %.1f\nCoords 2: %.1f, %.1f, %.1f\nMin xy: %d, %d\nMax xy: %d, %d\n", 
           screenCoords[0].x, screenCoords[0].y, screenCoords[0].z, 
           screenCoords[1].x, screenCoords[1].y, screenCoords[1].z, 
           screenCoords[2].x, screenCoords[2].y, screenCoords[2].z, 
           minX, minY, 
           maxX, maxY);
    */
    
    // Triangle setup.
    int A01 = screenCoords[0].y - screenCoords[1].y, B01 = screenCoords[1].x - screenCoords[0].x;
    int A12 = screenCoords[1].y - screenCoords[2].y, B12 = screenCoords[2].x - screenCoords[1].x;
    int A20 = screenCoords[2].y - screenCoords[0].y, B20 = screenCoords[0].x - screenCoords[2].x;

    // Determine barycentric coordinates.
    Vector3 p = { (float)minX, (float)minY, 0 };
    int w0_row = barycentricCoords(screenCoords[1], screenCoords[2], p);
    int w1_row = barycentricCoords(screenCoords[2], screenCoords[0], p);
    int w2_row = barycentricCoords(screenCoords[0], screenCoords[1], p);

    // Rasterize the triangle.
    for (p.y = minY; p.y <= maxY; p.y++) 
    {
        // Barycentric coordinates at start of row.
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        for (p.x = minX; p.x <= maxX; p.x++) 
        {
            // Make the barycentric coordinates percentages.
            float w0n = w0 / (float)(w0 + w1 + w2);
            float w1n = w1 / (float)(w0 + w1 + w2);
            float w2n = w2 / (float)(w0 + w1 + w2);

            // Calculate the pixel's color. (This is not very efficient but will probably be disabled once we implement textures)
            Color pCol = { vertices[0].color.r * w0n + vertices[1].color.r * w1n + vertices[2].color.r * w2n, 
                           vertices[0].color.g * w0n + vertices[1].color.g * w1n + vertices[2].color.g * w2n, 
                           vertices[0].color.b * w0n + vertices[1].color.b * w1n + vertices[2].color.b * w2n, 
                           vertices[0].color.a * w0n + vertices[1].color.a * w1n + vertices[2].color.a * w2n };

            // If p is on or inside all edges, render pixel.
            if ((w0 | w1 | w2) >= 0)
                drawPixel(p.x, p.y, pCol);

            // One step to the right.
            w0 += A12;
            w1 += A20;
            w2 += A01;
        }

        // One row step.
        w0_row += B12;
        w1_row += B20;
        w2_row += B01;
    }
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
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);
    ImGui::Text("\nMatrices:");
    ImGui::Text(("Model:\n"      + modelMat.printStr     (false)).c_str());
    ImGui::Text(("View:\n"       + viewMat.printStr      (false)).c_str());
    ImGui::Text(("Projection:\n" + projectionMat.printStr(false)).c_str());
}
