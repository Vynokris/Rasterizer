#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>

#include "Light.hpp"
#include "Renderer.hpp"

using namespace arithmetic;
using namespace geometry3D;

Renderer::Renderer(const unsigned int& _width, const unsigned int& _height, const std::vector<Light>& _lights)
        : viewport(0, 0, _width, _height)
        , lights(_lights)
        , framebuffer(_width, _height)
{ }

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

void Renderer::drawPixel(const unsigned int& _x, const unsigned int& _y, const float& _depth, const Color& _color)
{
    int index = _y * framebuffer.getWidth() + _x;
    
    if (_depth <= framebuffer.depthBuffer[index])
    {
        framebuffer.depthBuffer[index] = _depth;
        switch (getRenderMode())
        {
        case RenderMode::UNLIT:
        case RenderMode::LIT:
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
            drawPixel(point.x, point.y, lerp(lerpFactor, _p0.pos.z, _p1.pos.z), colorLerp(lerpFactor, _p0.color, _p1.color));
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

static void swapTriangleVertices(Vector3* _screenCoords, Vector4* _viewCoords, Vertex* _vertices)
{
    // Get the triangle's sides and normals.
    Vector2 p0p1(Vector2{ _screenCoords[0].x, _screenCoords[0].y }, Vector2{ _screenCoords[1].x, _screenCoords[1].y });
    Vector2 p1p2(Vector2{ _screenCoords[1].x, _screenCoords[1].y }, Vector2{ _screenCoords[2].x, _screenCoords[2].y });
    Vector2 p2p0(Vector2{ _screenCoords[2].x, _screenCoords[2].y }, Vector2{ _screenCoords[0].x, _screenCoords[0].y });
    float angle0 = p0p1.getAngleWithVector2(p1p2.getNormal());
    float angle1 = p1p2.getAngleWithVector2(p2p0.getNormal());
    float angle2 = p2p0.getAngleWithVector2(p0p1.getNormal());

    // Check if the triangle is inside out.
    if (angle0 < PI/2)
    {
        // Switch position, depth and vertices.
        Vector3 tempVec  = _screenCoords[0];
        _screenCoords[0] = _screenCoords[1];
        _screenCoords[1] = tempVec;
        float tempDepth  = _viewCoords[0].z;
        _viewCoords[0].z = _viewCoords[1].z;
        _viewCoords[1].z = tempDepth;
        Vertex tempVertex = _vertices[0];
        _vertices[0] = _vertices[1];
        _vertices[1] = tempVertex;
    }
    else if (angle1 < PI/2)
    {
        // Switch position, depth and vertices.
        Vector3 tempVec  = _screenCoords[1];
        _screenCoords[1] = _screenCoords[2];
        _screenCoords[2] = tempVec;
        float tempDepth  = _viewCoords[1].z;
        _viewCoords[1].z = _viewCoords[2].z;
        _viewCoords[2].z = tempDepth;
        Vertex tempVertex = _vertices[1];
        _vertices[1] = _vertices[2];
        _vertices[2] = tempVertex;
    }
    else if (angle2 < PI/2)
    {
        // Switch position, depth and vertices.
        Vector3 tempVec  = _screenCoords[2];
        _screenCoords[2] = _screenCoords[0];
        _screenCoords[0] = tempVec;
        float tempDepth  = _viewCoords[2].z;
        _viewCoords[2].z = _viewCoords[0].z;
        _viewCoords[0].z = tempDepth;
        Vertex tempVertex = _vertices[2];
        _vertices[2] = _vertices[0];
        _vertices[0] = tempVertex;
    }
}

bool Renderer::transformVertices(int count, Vertex* vertices, Vector3* local, Vector4* world, Vector4* view, Vector4* clip, Vector3* ndc, Vector3* screen)
{
    for (int i = 0; i < count; i++)
    {
        // Store triangle vertices positions.
        local[i] = vertices[i].pos;
        
        // Local space (3D) -> World space (3D).
        world[i] = Vector4{ local[i], 1 } * modelMat.back();
        
        // World space (3D) -> View space (3D).
        view[i] = world[i] * viewMat;
        
        // View space (3D) -> Clip space (4D).
        clip[i] = view[i] * projectionMat;

        // TODO: fix clipping.

        //! Temporarily clip triangles by nuking them when one vertex is offscreen.
        if (!((-abs(clip[i].w) <= clip[i].x) && (clip[i].x <= abs(clip[i].w)) &&
              (-abs(clip[i].w) <= clip[i].y) && (clip[i].y <= abs(clip[i].w)) &&
              (-abs(clip[i].w) <= clip[i].z) && (clip[i].z <= abs(clip[i].w))))
            return false;
        
        // Clip space (4D) -> NDC (3D).
        ndc[i] = clip[i].toVector3(true);
        
        // NDC (3D) -> screen coords (2D).
        screen[i] = ndcToScreenCoords(ndc[i], viewport);
    }

    return true;
}

void Renderer::drawTriangle(Triangle3 _triangle)
{
    Vector3 localCoords[3];
    Vector4 worldCoords[3];
    Vector4 viewCoords[3];
    Vector4 clipCoords[3];
    Vector3 ndcCoords[3];
    Vector3 screenCoords[3];

    if (!transformVertices(3, &_triangle.a, localCoords, worldCoords, viewCoords, clipCoords, ndcCoords, screenCoords))
        return;

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

    // Make sure the triangle vertices are in the right order to be drawn.
    swapTriangleVertices(screenCoords, viewCoords, &_triangle.a);

    // Bring uv coords to clip space.
    Vector3 perspectiveUV[3] = {
        { _triangle.a.uv.x / viewCoords[0].z, _triangle.a.uv.y / viewCoords[0].z, 1 / viewCoords[0].z },
        { _triangle.b.uv.x / viewCoords[1].z, _triangle.b.uv.y / viewCoords[1].z, 1 / viewCoords[1].z },
        { _triangle.c.uv.x / viewCoords[2].z, _triangle.c.uv.y / viewCoords[2].z, 1 / viewCoords[2].z }
    };

    // Determine the view vector and normals in world space.
    Vector3 viewVec = { -viewMat[3][0], -viewMat[3][1], -viewMat[3][2] };
    Vector3 worldNormals[3] = {
        (Vector4( _triangle.a.normal, 1 ) * modelMat.back()).getNormalized().toVector3(),
        (Vector4( _triangle.b.normal, 1 ) * modelMat.back()).getNormalized().toVector3(),
        (Vector4( _triangle.c.normal, 1 ) * modelMat.back()).getNormalized().toVector3()
    };

    // Compute Phong lighting for each vertex.
    Color lightIntensity[3] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, };
    for (Light it : lights)
    {
        for (int i = 0; i < 3; i++)
        {
            if (getRenderMode() == RenderMode::LIT)
            {
                lightIntensity[i] *= computePhong(it, mat, worldCoords[i].toVector3(), worldNormals[i], viewVec);
            }
            else
            {
                lightIntensity[i] = WHITE;
            }
        }
    }
    
    //! DEBUG PANNEL
    ImGui::Begin("Debug info");
    {
        for (int i = 0; i < 3; i++) ImGui::Text("Local  coords %d: (%.2f, %.2f, %.2f)%s",       i, localCoords[i].x,  localCoords[i].y,  localCoords[i].z,                    (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("World  coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, worldCoords[i].x,  worldCoords[i].y,  worldCoords[i].z,  worldCoords[i].w, (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("View   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, viewCoords[i].x,   viewCoords[i].y,   viewCoords[i].z,   viewCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("View vector: (%.2f, %.2f, %.2f)%s",            viewVec.x, viewVec.y, viewVec.z,                                              (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Clip   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, clipCoords[i].x,   clipCoords[i].y,   clipCoords[i].z,   clipCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("NDC    coords %d: (%.2f, %.2f, %.2f)%s",       i, ndcCoords[i].x,    ndcCoords[i].y,    ndcCoords[i].z,                      (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Screen coords %d: (%.0f, %.0f, %.3f)%s",       i, screenCoords[i].x, screenCoords[i].y, screenCoords[i].z,                   (i == 2 ? "\n " : ""));
    }
    ImGui::End();

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

            // Compute the pixel depth.
            float depth = abs(perspectiveUV[0].z * w0n + perspectiveUV[1].z * w1n + perspectiveUV[2].z * w2n);
                  depth = 1 / depth;

            // Define the pixel color.
            Color pCol { 0, 0, 0, 0 };

            if (texture.pixels == nullptr || texture.applyVertexColor)
            {
                // Get the pixel color from barycentric coordinates.
                pCol.r = _triangle.a.color.r * w0n * lightIntensity[0].r + _triangle.b.color.r * w1n * lightIntensity[0].g + _triangle.c.color.r * w2n * lightIntensity[0].b; 
                pCol.g = _triangle.a.color.g * w0n * lightIntensity[1].r + _triangle.b.color.g * w1n * lightIntensity[1].g + _triangle.c.color.g * w2n * lightIntensity[1].b; 
                pCol.b = _triangle.a.color.b * w0n * lightIntensity[2].r + _triangle.b.color.b * w1n * lightIntensity[2].g + _triangle.c.color.b * w2n * lightIntensity[2].b; 
                pCol.a = _triangle.a.color.a * w0n + _triangle.b.color.a * w1n + _triangle.c.color.a * w2n;
            }

            if (texture.pixels != nullptr)
            {
                // Compute the uv coordinates.
                Vector2 uv = { clamp(depth * (perspectiveUV[0].x * w0n + perspectiveUV[1].x * w1n + perspectiveUV[2].x * w2n), 0, 1),
                               clamp(depth * (perspectiveUV[0].y * w0n + perspectiveUV[1].y * w1n + perspectiveUV[2].y * w2n), 0, 1) };

                // Get the pixel color from the current texture.
                Color texColor = texture.getPixelColor(floorInt(clampAbove(lerp(uv.x, 0, abs(texture.width )), 0)), 
                                                       floorInt(clampAbove(lerp(uv.y, 0, abs(texture.height)), 0)));

                // Apply the texture color to the pixel color.
                if (texture.applyVertexColor)
                {
                    HSV pHSV = RGBtoHSV(texColor);
                    pCol = HSVtoRGB({ pCol.getHue(), pHSV.s, pHSV.v });
                }
                else
                {
                    pCol = texColor;
                }
            }

            if (p.x > maxX-1 && p.y > maxY-1)
            {
                ImGui::Begin("Debug info");
                ImGui::Text("Pixel depth: %f", depth);
                ImGui::Text("Pixel uv:    %f, %f", _triangle.a.uv.x * w0n + _triangle.b.uv.x * w1n + _triangle.c.uv.x * w2n, _triangle.a.uv.y * w0n + _triangle.b.uv.y * w1n + _triangle.c.uv.y * w2n);
                ImGui::Text("Pixel color: %.2f, %.2f, %.2f", pCol.r, pCol.g, pCol.b);
                ImGui::Text("Pixel hue:   %.2f", pCol.getHue());
                ImGui::Text("\n\n");
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
            { { -_size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 0 } },
            { {  _size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 0 } },
            { {  _size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 1 } },
        },
        {
            { { -_size / 2, -_size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 0 } },
            { { -_size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 1, 1 } },
            { {  _size / 2,  _size / 2, 0 }, { 0, 0, (_negateNormals ? 1.f : -1.f) }, _color, { 0, 1 } },
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

RenderMode Renderer::getRenderMode() const            { return renderMode; }
void Renderer::setRenderMode(const RenderMode& _mode) { renderMode = _mode; }

// --- Material and texture setters --- //

void Renderer::setTexture (const TextureData& _textureData) { texture = _textureData; }
void Renderer::setMaterial(const Material& _material)       { mat     = _material;    }

// ---------- Miscellaneous ---------- //

void Renderer::showImGuiControls()
{
    // Static variables.
    static const char* items[]{"Unlit", "Lit", "Wireframe", "Z-Buffer"};
    static int curItem = 0;
    
    // Displaying components.
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);
    ImGui::Combo("Render Mode", &curItem, items, IM_ARRAYSIZE(items));
    setRenderMode((RenderMode)curItem);
    ImGui::Checkbox("Apply vertex colors to texture", &texture.applyVertexColor);
    ImGui::Text("\nMatrices:");
    ImGui::Text("Model:\n%s",      modelMat.back().printStr(false).c_str());
    ImGui::Text("View:\n%s",       viewMat.printStr        (false).c_str());
    ImGui::Text("Projection:\n%s", projectionMat.printStr  (false).c_str());
}
