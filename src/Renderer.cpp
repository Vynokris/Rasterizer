#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>
using namespace arithmetic;
using namespace geometry3D;

#include "Renderer.hpp"

Renderer::Renderer(const unsigned int& _width, const unsigned int& _height)
        : viewport(0, 0, _width, _height)
        , framebuffer(_width, _height)
{}

Renderer::~Renderer()
{}

// -- Setters for the three matrices -- //

void Renderer::setModel(const Mat4& _modelMatrix)           { modelMat.clear(); modelMat.push_back(_modelMatrix); }
void Renderer::setView(const Mat4& _viewMatrix)             { viewMat = _viewMatrix;                              }
void Renderer::setProjection(const Mat4& _projectionMatrix) { projectionMat = _projectionMatrix;                  }

// ------- Model transformations ------ //

void Renderer::modelPushMat  ()                                                                 { modelMat.push_back(modelMat.back());                                               }
void Renderer::modelPopMat   ()                                                                 { if (modelMat.size() > 1) modelMat.pop_back();                                      }
void Renderer::modelTranslate(const float& _x, const float& _y, const float& _z)                { modelMat.back() = getTranslationMatrix({ _x, _y, _z })          * modelMat.back(); }
void Renderer::modelRotateX  (const float& _angle)                                              { modelMat.back() = getXRotationMatrix(_angle)                    * modelMat.back(); }
void Renderer::modelRotateY  (const float& _angle)                                              { modelMat.back() = getYRotationMatrix(_angle)                    * modelMat.back(); }
void Renderer::modelRotateZ  (const float& _angle)                                              { modelMat.back() = getZRotationMatrix(_angle)                    * modelMat.back(); }
void Renderer::modelScale    (const float& _scaleX, const float& _scaleY, const float& _scaleZ) { modelMat.back() = getScaleMatrix({ _scaleX, _scaleY, _scaleZ }) * modelMat.back(); }

// --------- Drawing functions -------- //

void Renderer::setTexture(float* _colors32Bits, const int& _width, const int& _height)
{
    // TODO
}

void Renderer::drawPixel(const unsigned int& _x, const unsigned int& _y, const float& _depth, const Color& _color)
{
    int index = _y * framebuffer.getWidth() + _x;
    
    if (_depth <= framebuffer.depthBuffer[index])
    {
        framebuffer.depthBuffer[index] = _depth;
        switch (getRenderMode())
        {
        case RenderMode::DEFAULT:
        case RenderMode::WIREFRAME: framebuffer.colorBuffer[index] = _color;                        break;
        case RenderMode::ZBUFFER:   framebuffer.colorBuffer[index] = { _depth, _depth, _depth, 1 }; break;
        default: break;
        }
    }
}

void Renderer::drawLine(const Vertex& _p0, const Vertex& _p1)
{
    // Get the distance between the two points
    int dx =  abs(_p1.pos.x - _p0.pos.x);
    int dy = -abs(_p1.pos.y - _p0.pos.y);

    // Create a point to be moved along the line.
    Vector2 point = { _p0.pos.x, _p0.pos.y };

    // Calculate the slope increments along the x and y axes.
    int sx = _p0.pos.x < _p1.pos.x ? 1 : -1;
    int sy = _p0.pos.y < _p1.pos.y ? 1 : -1;

    // Compute the error margins.
    int err = dx + dy;
    int e2;

    while (true)
    {
        //? This if statement will probably be avoidable one we get the cliping right.
        if (0 <= point.x && point.x < viewport.width && 
            0 <= point.y && point.y < viewport.height)
        {
            float lerpFactor = (getLerp(point.x, _p0.pos.x, _p1.pos.x) + getLerp(point.y, _p0.pos.y, _p1.pos.y)) / 2;
            drawPixel(point.x, point.y, 0, colorLerp(lerpFactor, _p0.color, _p1.color));
        }

        e2 = 2 * err;
        if (e2 >= dy)
        {
            // Stop if the first point reached the second one.
            if (roundInt(point.x) == roundInt(_p1.pos.x)) break;

            // Increment the error and the first point's x coordinate.
            err += dy; point.x += sx;
        }
        if (e2 <= dx)
        {
            // Stop if the first point reached the second one.
            if (roundInt(point.y) == roundInt(_p1.pos.y)) break;

            // Increment the error and the first point's y coordinate.
            err += dx; point.y += sy;
        }
    }
}

static Vector3 ndcToScreenCoords(const Vector3& _ndc, const Viewport& _viewport)
{
    return {
        _ndc.x * _viewport.width  + _viewport.width  / 2, 
        _ndc.y * _viewport.height + _viewport.height / 2, 
        _ndc.z
    };
}

static int barycentricCoords(const Vector3& _a, const Vector3& _b, const Vector3& _c)
{
    return (_b.x - _a.x) * (_c.y - _a.y) - (_b.y - _a.y) * (_c.x - _a.x);
}

void Renderer::drawTriangle(Triangle3 _triangle)
{
    Vector3 localCoords[3];
    Vector4 worldCoords[3];
    Vector4 viewCoords[3];
    Vector4 clipCoords[3];

    // Store triangle vertices positions.
    localCoords[0] = _triangle.a.pos;
    localCoords[1] = _triangle.b.pos;
    localCoords[2] = _triangle.c.pos;
    
    // Local space (3D) -> World space (3D).
    worldCoords[0] = { (Vector4{ localCoords[0], 1 } * modelMat.back()) };
    worldCoords[1] = { (Vector4{ localCoords[1], 1 } * modelMat.back()) };
    worldCoords[2] = { (Vector4{ localCoords[2], 1 } * modelMat.back()) };
    
    // World space (3D) -> View space (3D).
    viewCoords[0] = { (worldCoords[0] * viewMat) };
    viewCoords[1] = { (worldCoords[1] * viewMat) };
    viewCoords[2] = { (worldCoords[2] * viewMat) };
    
    // View space (3D) -> Clip space (4D).
    clipCoords[0] = { viewCoords[0] * projectionMat };
    clipCoords[1] = { viewCoords[1] * projectionMat };
    clipCoords[2] = { viewCoords[2] * projectionMat };

    // TODO: fix clipping.
    /*
    if (!wasClipped)
    {
        // Clip the triangle against the frustum.
        Triangle3 viewTriangle(
            { clipCoords[0].toVector3(), _triangle.a.normal, _triangle.a.color, _triangle.a.uv }, 
            { clipCoords[1].toVector3(), _triangle.b.normal, _triangle.b.color, _triangle.b.uv }, 
            { clipCoords[2].toVector3(), _triangle.c.normal, _triangle.c.color, _triangle.c.uv }
        );
        float vertexAbsW[3] = { abs(clipCoords[0].w), abs(clipCoords[1].w), abs(clipCoords[2].w) };
        std::vector<Triangle3> clippedTriangles = clipHomogeneousTriangle(viewTriangle, vertexAbsW);
        
        // Draw the clipped triangles.
        for (int i = 0; i < (int)clippedTriangles.size(); i++)
            drawTriangle(clippedTriangles[i]);
        return;
    }
    */

    //! Temporarily clip triangles by nuking them when one vertex is offscreen.
    /*
    if (clipCoords[0].w >= 0 ||
        clipCoords[1].w >= 0 ||
        clipCoords[2].w >= 0)
        return;
    */
   
    for (int i = 0; i < 3; i++)
    {
        if (!(-abs(clipCoords[i].w) <= clipCoords[i].x && clipCoords[i].x <= abs(clipCoords[i].w) &&
              -abs(clipCoords[i].w) <= clipCoords[i].y && clipCoords[i].y <= abs(clipCoords[i].w) &&
              -abs(clipCoords[i].w) <= clipCoords[i].z && clipCoords[i].z <= abs(clipCoords[i].w)))
        {
            return;
        }
    }
    
    // Clip space (4D) -> NDC (3D).
    Vector3 ndcCoords[3] = {
        { clipCoords[0].toVector3(true) },
        { clipCoords[1].toVector3(true) },
        { clipCoords[2].toVector3(true) }
    };

    // TODO: fix clipping.
    /*
    if (wasClipped)
    {
        ndcCoords[0] = _triangle.a.pos;
        ndcCoords[1] = _triangle.b.pos;
        ndcCoords[2] = _triangle.c.pos;
    }
    */
    
    // NDC (3D) -> screen coords (2D).
    Vector3 screenCoords[3] = {
        { ndcToScreenCoords(ndcCoords[0], viewport) },
        { ndcToScreenCoords(ndcCoords[1], viewport) },
        { ndcToScreenCoords(ndcCoords[2], viewport) },
    };

    ImGui::Begin("Debug info");
    {
        for (int i = 0; i < 3; i++) ImGui::Text("Local  coords %d: (%.2f, %.2f, %.2f)%s",       i, localCoords[i].x,  localCoords[i].y,  localCoords[i].z,                    (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("World  coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, worldCoords[i].x,  worldCoords[i].y,  worldCoords[i].z,  worldCoords[i].w, (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("View   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, viewCoords[i].x,   viewCoords[i].y,   viewCoords[i].z,   viewCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Clip   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, clipCoords[i].x,   clipCoords[i].y,   clipCoords[i].z,   clipCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("NDC    coords %d: (%.2f, %.2f, %.2f)%s",       i, ndcCoords[i].x,    ndcCoords[i].y,    ndcCoords[i].z,                      (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Screen coords %d: (%.0f, %.0f, %.3f)%s",       i, screenCoords[i].x, screenCoords[i].y, screenCoords[i].z,                   (i == 2 ? "\n " : ""));
        ImGui::Text("\n");
    }
    ImGui::End();

    // Draw triangle wireframe
    if (getRenderMode() == RenderMode::WIREFRAME)
    {
        drawLine({ screenCoords[0], _triangle.a.normal, _triangle.a.color, _triangle.a.uv }, 
                 { screenCoords[1], _triangle.b.normal, _triangle.b.color, _triangle.b.uv });
        drawLine({ screenCoords[1], _triangle.b.normal, _triangle.b.color, _triangle.b.uv }, 
                 { screenCoords[2], _triangle.c.normal, _triangle.c.color, _triangle.c.uv });
        drawLine({ screenCoords[2], _triangle.c.normal, _triangle.c.color, _triangle.c.uv }, 
                 { screenCoords[0], _triangle.a.normal, _triangle.a.color, _triangle.a.uv });
        return;
    }

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
            Color pCol = { _triangle.a.color.r * w0n + _triangle.b.color.r * w1n + _triangle.c.color.r * w2n, 
                           _triangle.a.color.g * w0n + _triangle.b.color.g * w1n + _triangle.c.color.g * w2n, 
                           _triangle.a.color.b * w0n + _triangle.b.color.b * w1n + _triangle.c.color.b * w2n, 
                           _triangle.a.color.a * w0n + _triangle.b.color.a * w1n + _triangle.c.color.a * w2n };

            // Compute depth
            // TODO: Wrong way to compute depth
            //? See: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation
            float depth = abs(viewCoords[0].z * w0n + viewCoords[1].z * w1n + viewCoords[2].z * w2n);

            if (p.x >= maxX-1 && p.y >= maxY-1)
            {
                ImGui::Begin("Debug info");
                ImGui::Text("Pixel depth: %f", depth);
                ImGui::End();
            }

            // If p is on or inside all edges, render pixel.
            if ((w0 | w1 | w2) >= 0) drawPixel(p.x, p.y, depth, pCol);

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

void Renderer::drawTriangles(Triangle3* _triangles, const unsigned int& _count)
{
    for (int i = 0; i < (int)_count; i++) drawTriangle(_triangles[i]);
}


void Renderer::drawDividedQuad(const Color& _color, const float& _size, const bool& _negateNormals)
{
    Triangle3 triangles[2] = 
    {
        {
            { { -_size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 0 } },
            { { -_size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 1 } },
            { {  _size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 0 } },
        },
        {
            { {  _size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 1 } },
            { {  _size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 0 } },
            { { -_size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 1 } },
        },
    };
    
    drawTriangles(triangles, 2);
}

void Renderer::drawCube(const Color& _color, const float& _size)
{
    // Render all the faces.
    for (int i = 0; i < 6; i++)
    {
        if      (i < 4)  modelRotateY(PI / 2);
        else if (i == 4) modelRotateX(PI / 2);
        else if (i == 5) modelRotateX(PI);

        modelPushMat();
        modelTranslate(0, 0, _size / 2);
        drawDividedQuad(_color, _size, true);
        modelPopMat();
    }
}

void Renderer::drawSphere(const float& _r, const int& _lon, const int& _lat, const Color& _color)
{
    for (int j = 0; j < _lat; j++)
    {
        float theta0 = ( (j + 0) / (float)_lat) * PI;
        float theta1 = ( (j + 1) / (float)_lat) * PI;

        for (int i = 0; i < _lon; i++)
        {
            float phi0 = ( (i + 0) / (float)_lon) * 2.f * PI;
            float phi1 = ( (i + 1) / (float)_lon) * 2.f * PI;

            Vector3 c0 = getSphericalCoords(_r, theta0, phi0);
            Vector3 c1 = getSphericalCoords(_r, theta0, phi1);
            Vector3 c2 = getSphericalCoords(_r, theta1, phi1);
            Vector3 c3 = getSphericalCoords(_r, theta1, phi0);

            Triangle3 triangles[2] = 
            {
                {
                    { c0, { 0, 0, 1 }, _color, { 0, 0 } },
                    { c1, { 0, 0, 1 }, _color, { 0, 1 } },
                    { c2, { 0, 0, 1 }, _color, { 1, 0 } },
                },
                {
                    { c0, { 0, 0, 1 }, _color, { 1, 1 } },
                    { c2, { 0, 0, 1 }, _color, { 1, 0 } },
                    { c3, { 0, 0, 1 }, _color, { 0, 1 } },
                }
            };
    
            drawTriangles(triangles, 2);
        }
    }
}

// --- View mode getters / setters --- //

RenderMode Renderer::getRenderMode() const
{
    return renderMode;
}

void Renderer::setRenderMode(const RenderMode& _mode)
{
    renderMode = _mode;
}

// ---------- Miscellaneous ---------- //

void Renderer::showImGuiControls()
{
    // Static variables.
    static const char* items[]{"Default", "Wireframe", "Z-Buffer"};
    static int curItem = 0;
    
    // Displaying components.
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);
    ImGui::Combo("Render Mode", &curItem, items, IM_ARRAYSIZE(items));
    setRenderMode((RenderMode)curItem);
    ImGui::Text("\nMatrices:");
    ImGui::Text("Model:\n%s",      modelMat.back().printStr(false).c_str());
    ImGui::Text("View:\n%s",       viewMat.printStr        (false).c_str());
    ImGui::Text("Projection:\n%s", projectionMat.printStr  (false).c_str());
}
