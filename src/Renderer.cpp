#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>

#include "Light.hpp"
#include "Renderer.hpp"

using namespace std;
using namespace arithmetic;
using namespace geometry3D;

Renderer::Renderer(const unsigned int& _width, const unsigned int& _height, vector<Light>* _lights)
        : viewport(0, 0, _width, _height)
        , lights(_lights)
        , framebuffer(_width, _height)
{ }

// -- Setters for the three matrices -- //

Mat4& Renderer::getModel()                                   { return modelMat.back(); }
void  Renderer::setModel(const Mat4& _modelMatrix)           { modelMat.clear(); modelMat.push_back(_modelMatrix); }
void  Renderer::setView(const Mat4& _viewMatrix)             { viewMat = _viewMatrix;                              }
void  Renderer::setProjection(const Mat4& _projectionMatrix) { projectionMat = _projectionMatrix;                  }

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
        case RenderMode::ZBUFFER:   framebuffer.colorBuffer[index] = { _depth, _depth, _depth, 1 }; break;
        default:                    framebuffer.colorBuffer[index] = _color;                        break;
        }
    }
}

void Renderer::drawLine(const Vertex& _p0, const Vertex& _p1)
{
    // Get the distance between the two points.
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
    return { _ndc.x * _viewport.width  + _viewport.width  / 2, 
             _ndc.y * _viewport.height + _viewport.height / 2, 
             _ndc.z };
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
    int a = 0, b = 0;

    if      (angle0 < PI/2) { a = 0; b = 1; }
    else if (angle1 < PI/2) { a = 1; b = 2; }
    else if (angle2 < PI/2) { a = 2; b = 0; }

    // Swap position, depth and vertices.
    Vector3 tempVec   = _screenCoords[a]; _screenCoords[a] = _screenCoords[b]; _screenCoords[b] = tempVec;
    float tempDepth   = _viewCoords[a].z; _viewCoords[a].z = _viewCoords[b].z; _viewCoords[b].z = tempDepth;
    Vertex tempVertex = _vertices[a];     _vertices[a]     = _vertices[b];     _vertices[b]     = tempVertex;
}

bool Renderer::transformVertices(int _count, Vertex* _vertices, Vector3* _local, Vector4* _world, Vector4* _view, Vector4* _clip, Vector3* _ndc, Vector3* _screen, Vector3* _perspectiveUV)
{
    for (int i = 0; i < _count; i++)
    {
        _local[i] = _vertices[i].pos;                          // Store triangle vertices positions.
        _world[i] = Vector4{ _local[i], 1 } * modelMat.back(); // Local space (3D) -> World space (3D).
        _view[i]  = _world[i] * viewMat;                       // World space (3D) -> View space (3D).
        _clip[i]  = _view[i] * projectionMat;                  // View space (3D) -> Clip space (4D).

        // TODO: fix clipping.
        //! Temporarily clip triangles by nuking them when one vertex is offscreen.
        if (!((-abs(_clip[i].w) <= _clip[i].x) && (_clip[i].x <= abs(_clip[i].w)) &&
              (-abs(_clip[i].w) <= _clip[i].y) && (_clip[i].y <= abs(_clip[i].w)) &&
              (-abs(_clip[i].w) <= _clip[i].z) && (_clip[i].z <= abs(_clip[i].w))))
            return false;
        
        
        _ndc[i]    = _clip[i].toVector3(true);             // Clip space (4D) -> NDC (3D).
        _screen[i] = ndcToScreenCoords(_ndc[i], viewport); // NDC (3D) -> screen coords (2D).
    }
    
    // Make sure the triangle vertices are in the right order to be drawn.
    swapTriangleVertices(_screen, _view, _vertices);
    
    // Bring uv coords to clip space.
    for (int i = 0; i < _count; i++)
        _perspectiveUV[i] = { _vertices[i].uv.x / _view[i].z, _vertices[i].uv.y / _view[i].z, 1 / _view[i].z };

    return true;
}

static bool isTowardsCamera(const Vector3 _worldPos, const Vector3& _worldNormal, const Vector3& _camPos)
{
    return (_worldNormal & Vector3(_camPos, _worldPos)) <= 0;
}

void Renderer::drawTriangle(Triangle3 _triangle, TextureData* _texture)
{
    // Create all coordinates systems arrays.
    Vector3 localCoords  [3];
    Vector4 worldCoords  [3];
    Vector4 viewCoords   [3];
    Vector4 clipCoords   [3];
    Vector3 ndcCoords    [3];
    Vector3 screenCoords [3];
    Vector3 perspectiveUV[3];

    // Get the triangle's world position.
    Vector3 trianglePos = (Vector4(_triangle.getCenterOfMass().pos, 1) 
                        * modelMat.back()).toVector3();

    // Get the triangle's normal in world coordinates.
    Vector3 worldNormal = (Vector4((_triangle.a.normal + _triangle.b.normal + _triangle.c.normal) / 3, 0)
                        * modelMat.back()).toVector3().getNormalized();

    // Get the camera's position.
    Vector3 cameraPos = (Vector4(0, 0, 0, 1)
                      * viewMat.inv4()).toVector3();

    //! DEBUG PANNEL
    ImGui::Begin("Debug info");
    ImGui::Text("Triangle position:  %.2f, %.2f, %.2f", trianglePos.x, trianglePos.y, trianglePos.z);
    ImGui::Text("Triangle normal:    %.2f, %.2f, %.2f", worldNormal.x, worldNormal.y, worldNormal.z);
    ImGui::Text("Vector to triangle: %.2f, %.2f, %.2f", Vector3(cameraPos, trianglePos).x, Vector3(cameraPos, trianglePos).y, Vector3(cameraPos, trianglePos).z);
    ImGui::Text("Camera position:    %.2f, %.2f, %.2f", cameraPos.x, cameraPos.y, cameraPos.z);
    ImGui::Text("Dot product:        %.2f",             worldNormal & Vector3(cameraPos, trianglePos));
    ImGui::Text("\n");
    ImGui::End();

    // Back face culling.
    if (!isTowardsCamera(trianglePos, worldNormal, cameraPos))
        return;

    // Transform the triangle's vertices through the renderer's matrices.
    if (!transformVertices(3, &_triangle.a, localCoords, worldCoords, viewCoords, clipCoords, ndcCoords, screenCoords, perspectiveUV))
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

    // Compute Phong lighting for each vertex.
    Color lightIntensity[3] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, };
    Material mat = { WHITE, WHITE, WHITE, 0, 0 };
    for (Light it : *lights)
    {
        for (int i = 0; i < 3; i++)
        {
            if (getRenderMode() == RenderMode::LIT) lightIntensity[i] *= computePhong(it, mat, worldCoords[i].toVector3(), worldNormal, cameraPos);
            else                                    lightIntensity[i] = WHITE;
        }
    }
    
    
    //! DEBUG PANNEL ======================================================
    ImGui::Begin("Debug info");
    {
        for (int i = 0; i < 3; i++) ImGui::Text("Local  coords %d: (%.2f, %.2f, %.2f)%s",       i, localCoords[i].x,  localCoords[i].y,  localCoords[i].z,                    (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("World  coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, worldCoords[i].x,  worldCoords[i].y,  worldCoords[i].z,  worldCoords[i].w, (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("View   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, viewCoords[i].x,   viewCoords[i].y,   viewCoords[i].z,   viewCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Clip   coords %d: (%.2f, %.2f, %.2f, %.2f)%s", i, clipCoords[i].x,   clipCoords[i].y,   clipCoords[i].z,   clipCoords[i].w,  (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("NDC    coords %d: (%.2f, %.2f, %.2f)%s",       i, ndcCoords[i].x,    ndcCoords[i].y,    ndcCoords[i].z,                      (i == 2 ? "\n " : ""));
        for (int i = 0; i < 3; i++) ImGui::Text("Screen coords %d: (%.0f, %.0f, %.3f)%s",       i, screenCoords[i].x, screenCoords[i].y, screenCoords[i].z,                   (i == 2 ? "\n " : ""));
    }
    ImGui::End();
    //! DEBUG PANNEL ======================================================

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
                
            // Interpolate pixel lighting.
            Color pLight { 
                w0n * lightIntensity[0].r + w1n * lightIntensity[0].g + w2n * lightIntensity[0].b,
                w0n * lightIntensity[1].r + w1n * lightIntensity[1].g + w2n * lightIntensity[1].b,
                w0n * lightIntensity[2].r + w1n * lightIntensity[2].g + w2n * lightIntensity[2].b,
            };

            // Define the pixel color.
            Color pCol { 0, 0, 0, 0 };

            if (_texture == nullptr || _texture->applyVertexColor)
            {
                // Get the pixel color from barycentric coordinates.
                pCol.r = _triangle.a.color.r * w0n + _triangle.b.color.r * w1n + _triangle.c.color.r * w2n; 
                pCol.g = _triangle.a.color.g * w0n + _triangle.b.color.g * w1n + _triangle.c.color.g * w2n; 
                pCol.b = _triangle.a.color.b * w0n + _triangle.b.color.b * w1n + _triangle.c.color.b * w2n; 
                pCol.a = _triangle.a.color.a * w0n + _triangle.b.color.a * w1n + _triangle.c.color.a * w2n;
            }

            if (_texture != nullptr)
            {
                // Compute the uv coordinates.
                Vector2 uv = { clamp(depth * (perspectiveUV[0].x * w0n + perspectiveUV[1].x * w1n + perspectiveUV[2].x * w2n), 0, 1),
                               clamp(depth * (perspectiveUV[0].y * w0n + perspectiveUV[1].y * w1n + perspectiveUV[2].y * w2n), 0, 1) };

                // Get the pixel color from the current texture.
                Color texColor = _texture->getPixelColor(floorInt(clampAbove(lerp(uv.x, 0, abs(_texture->width )), 0)), 
                                                       floorInt(clampAbove(lerp(uv.y, 0, abs(_texture->height)), 0)));

                // Apply the texture color to the pixel color.
                if (_texture->applyVertexColor)
                {
                    HSV pHSV = RGBtoHSV(texColor);
                    pCol = HSVtoRGB({ pCol.getHue(), pHSV.s, pHSV.v });
                }
                else
                {
                    pCol = texColor;
                }
            }

            // Apply the pixel lighting to the pixel color.
            pCol.r *= pLight.r;
            pCol.g *= pLight.g;
            pCol.b *= pLight.b;

            // If p is on or inside all edges, render pixel.
            if ((w0 | w1 | w2) >= 0) drawPixel(p.x, p.y, depth, pCol);

            //! Show debug info on the last pixel.
            if (p.x > maxX-1 && p.y > maxY-1)
            {
                ImGui::Begin("Debug info");
                ImGui::Text("Pixel depth: %f", depth);
                ImGui::Text("Pixel uv:    %f, %f", _triangle.a.uv.x * w0n + _triangle.b.uv.x * w1n + _triangle.c.uv.x * w2n, _triangle.a.uv.y * w0n + _triangle.b.uv.y * w1n + _triangle.c.uv.y * w2n);
                ImGui::Text("Pixel color: %.2f, %.2f, %.2f", pCol.r, pCol.g, pCol.b);
                ImGui::Text("Pixel hue:   %.2f", pCol.getHue());
                ImGui::Text("\n\n\n");
                ImGui::End();
            }

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

void Renderer::drawTriangles(Triangle3* _triangles, const unsigned int& _count, TextureData* _texture)
{
    for (int i = 0; i < (int)_count; i++) drawTriangle(_triangles[i], _texture);
}

//! Using meshes instead.
/*
void Renderer::drawDividedQuad(const Color& _color, const float& _size, const bool& _negateNormals)
{
    // Get the quad's normal.
    Vector3 normal = (Vector3({ -_size / 2, -_size / 2, 0 }, {  _size / 2, -_size / 2, 0 }) 
                   ^  Vector3({ -_size / 2, -_size / 2, 0 }, { -_size / 2,  _size / 2, 0 })).getNormalized();

    Triangle3 triangles[2] = 
    {
        {
            { { -_size / 2, -_size / 2, 0 }, normal, _color, { 1, 0 } },
            { {  _size / 2, -_size / 2, 0 }, normal, _color, { 0, 0 } },
            { {  _size / 2,  _size / 2, 0 }, normal, _color, { 0, 1 } },
        },
        {
            { { -_size / 2, -_size / 2, 0 }, normal, _color, { 1, 0 } },
            { { -_size / 2,  _size / 2, 0 }, normal, _color, { 1, 1 } },
            { {  _size / 2,  _size / 2, 0 }, normal, _color, { 0, 1 } },
        },
    };
    
    drawTriangles(triangles, 2);
}

void Renderer::drawCube(const Color& _color, const float& _size)
{
    // Render all the faces.
    modelPushMat();
    for (int i = 0; i < 6; i++)
    {
        if      (i < 4)  modelRotateY(PI / 2);
        else if (i == 4) modelRotateX(PI / 2);
        else if (i == 5) modelRotateX(PI);

        modelPushMat();
        modelTranslate(0, 0, _size / 2);
        drawDividedQuad(_color, _size, (i >= 4 ? true : false));
        modelPopMat();
    }
    modelPopMat();
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

            // Get the position of the 4 points of the current face.
            Vector3 c0 = getSphericalCoords(_r, theta0, phi0);
            Vector3 c1 = getSphericalCoords(_r, theta0, phi1);
            Vector3 c2 = getSphericalCoords(_r, theta1, phi1);
            Vector3 c3 = getSphericalCoords(_r, theta1, phi0);

            // Compute the face normal.
            Vector3 normal = (Vector3(c0, c1) ^ Vector3(c0, c2));
            if (normal.x == 0 && normal.y == 0 && normal.z == 0)
                normal = (Vector3(c0, c2) ^ Vector3(c0, c3));
            normal.normalize();

            // Create the face triangles.
            Triangle3 triangles[2] = 
            {
                {
                    { c0, normal, _color, { 0, 0 } },
                    { c1, normal, _color, { 0, 1 } },
                    { c2, normal, _color, { 1, 0 } },
                },
                {
                    { c0, normal, _color, { 1, 1 } },
                    { c2, normal, _color, { 1, 0 } },
                    { c3, normal, _color, { 0, 1 } },
                }
            };
    
            drawTriangles(triangles, 2);
        }
    }
}
*/

// ----- Render getters / setters ----- //

RenderMode Renderer::getRenderMode() const            { return renderMode; }
void Renderer::setRenderMode(const RenderMode& _mode) { renderMode = _mode; }

// ---------- Miscellaneous ---------- //

void Renderer::showImGuiControls()
{
    // Static variables.
    static const char* items[]{"Unlit", "Lit", "Wireframe", "Z-Buffer"};
    static int curItem = 1;
    
    // Displaying components.
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);
    ImGui::Combo("Render Mode", &curItem, items, IM_ARRAYSIZE(items));
    setRenderMode((RenderMode)curItem);
    ImGui::Text("\nMatrices:");
    ImGui::Text("Model:\n%s",      modelMat.back().printStr(false).c_str());
    ImGui::Text("View:\n%s",       viewMat.printStr        (false).c_str());
    ImGui::Text("Projection:\n%s", projectionMat.printStr  (false).c_str());
}
