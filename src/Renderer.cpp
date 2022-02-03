#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>

#include "Renderer.hpp"

Renderer::Renderer(float* p_colorBuffer32Bits, float* p_depthBuffer, const unsigned int p_width, const unsigned int p_height)
           : fb(p_width, p_height),viewport(0,0,p_width, p_height)
{
    //fb.colorBuffer = reinterpret_cast<float4*>(p_colorBuffer32Bits);
    //fb.depthBuffer = p_depthBuffer;

}

Renderer::~Renderer()
{

}

void Renderer::SetProjection(const Mat4& p_projectionMatrix)
{
    // TODO
}

void Renderer::SetView(const Mat4& p_viewMatrix)
{
    // TODO
}

void Renderer::SetModel(const Mat4& p_modelMatrix)
{
    // TODO
}

void Renderer::SetViewport(const int p_x, const int p_y, const unsigned int p_width, const unsigned int p_height)
{
    // TODO
}

void Renderer::SetTexture(float* p_colors32Bits, const unsigned int p_width, const unsigned int p_height)
{
    // TODO
}

void DrawPixel(Color* p_colorBuffer, unsigned int p_width, unsigned int p_height, unsigned int p_x, unsigned int p_y, Color p_color)
{
    // TODO
}
void Renderer::DrawLine(const Vector3& p0, const Vector3& p1, const Color& color)
{
   //TODO
}

Vector3 ndcToScreenCoords(Vector3 ndc, const Viewport& viewport)
{
    // TODO
    return ndc;
}

void Renderer::DrawTriangle(rdrVertex* vertices)
{
    // Store triangle vertices positions
    Vector3 localCoords[3] = {
        { vertices[0].x, vertices[0].y, vertices[0].z },
        { vertices[1].x, vertices[1].y, vertices[1].z },
        { vertices[2].x, vertices[2].y, vertices[2].z },
    };

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

    // Draw triangle wireframe
    DrawLine(screenCoords[0], screenCoords[1], lineColor);
    DrawLine(screenCoords[1], screenCoords[2], lineColor);
    DrawLine(screenCoords[2], screenCoords[0], lineColor);
}

void Renderer::DrawTriangles(rdrVertex* p_vertices, const unsigned int p_count)
{
    // calculate mvp from matrices
    // Transform vertex list to triangles into colorBuffer
    for (int i = 0; i < (int)p_count; i += 3)
    {
        DrawTriangle(&p_vertices[i]);
    }
}

void Renderer::ShowImGuiControls()
{
    ImGui::ColorEdit4("lineColor", lineColor.val);
}
