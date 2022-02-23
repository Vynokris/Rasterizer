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
{
    renderMode   = RenderMode::LIT;
    lightingMode = LightingMode::PHONG;
}

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

void Renderer::drawPixel(const unsigned int& _x, const unsigned int& _y, const float& _depth, Color _color)
{
    int    index  = _y * framebuffer.getWidth() + _x;
    float& bufferDepth = framebuffer.depthBuffer[index];
    Color& bufferColor = framebuffer.colorBuffer[index];
    bool   isCloser    = _depth < bufferDepth;

    // Alpha blending.
    bool blendAlpha = false;
    if (_color.a < 1 || bufferColor.a < 1)
    {
        blendAlpha = true;
        float alpha = (isCloser ? _color.a : (1 - bufferColor.a));
        _color = _color * alpha + bufferColor * (1 - alpha);
    }

    // Draw the pixel (color or depth) if it is closer than the previus one.
    if (isCloser || blendAlpha)
    {
        bufferDepth = _depth;
        switch (renderMode)
        {
        case RenderMode::ZBUFFER:   bufferColor = { _depth, _depth, _depth, 1 }; break;
        default:                    bufferColor = _color;                        break;
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
    return { _ndc.x * _viewport.width  + _viewport.width  / 2, 
             _ndc.y * _viewport.height + _viewport.height / 2, 
             _ndc.z };
}

static int barycentricCoords(const Vector3& _a, const Vector3& _b, const Vector3& _c)
{
    return (_b.x - _a.x) * (_c.y - _a.y) - (_b.y - _a.y) * (_c.x - _a.x);
}

static void swapTriangleVertices(Vector3* _screenCoords, Vector4* _worldCoords, Vector4* _viewCoords, Vertex* _vertices)
{
    // Get the triangle's sides and normals.
    Vector2 p0p1(Vector2{ _screenCoords[0].x, _screenCoords[0].y }, Vector2{ _screenCoords[1].x, _screenCoords[1].y });
    Vector2 p1p2(Vector2{ _screenCoords[1].x, _screenCoords[1].y }, Vector2{ _screenCoords[2].x, _screenCoords[2].y });
    Vector2 p2p0(Vector2{ _screenCoords[2].x, _screenCoords[2].y }, Vector2{ _screenCoords[0].x, _screenCoords[0].y });
    
    float angle0 = p0p1.getAngleWithVector2(p1p2.getNormal());
    float angle1 = p2p0.getAngleWithVector2(p0p1.getNormal());
    
    // Check if the triangle is inside out.
    if (angle0 < PI/2)
    {
        // Switch position, depth and vertices.
        Vector3 tempVec  = _screenCoords[0];
        _screenCoords[0] = _screenCoords[1];
        _screenCoords[1] = tempVec;

        Vector4 tempWorld = _worldCoords[0];
        _worldCoords[0]   = _worldCoords[1];
        _worldCoords[1]   = tempWorld;
        
        float tempDepth  = _viewCoords[0].z;
        _viewCoords[0].z = _viewCoords[1].z;
        _viewCoords[1].z = tempDepth;
        
        Vertex tempVertex = _vertices[0];
        _vertices[0]      = _vertices[1];
        _vertices[1]      = tempVertex;
    }
    else if (angle1 < PI/2)
    {
        // Switch position, depth and vertices.
        Vector3 tempVec  = _screenCoords[2];
        _screenCoords[2] = _screenCoords[0];
        _screenCoords[0] = tempVec;

        Vector4 tempWorld = _worldCoords[2];
        _worldCoords[2]   = _worldCoords[0];
        _worldCoords[0]   = tempWorld;

        float tempDepth  = _viewCoords[2].z;
        _viewCoords[2].z = _viewCoords[0].z;
        _viewCoords[0].z = tempDepth;
        
        Vertex tempVertex = _vertices[2];
        _vertices[2]      = _vertices[0];
        _vertices[0]      = tempVertex;
    }
}

bool Renderer::transformVertices(int _count, Vertex* _vertices, Vector3* _local, Vector4* _world, Vector4* _view, Vector4* _clip, Vector3* _ndc, Vector3* _screen, Vector3* _perspectiveUV)
{
    for (int i = 0; i < _count; i++)
    {
        // Store triangle vertices positions.
        _local[i] = _vertices[i].pos;
        
        // Local space (3D) -> World space (3D).
        _world[i] = Vector4{ _local[i], 1 } * modelMat.back();
        
        // World space (3D) -> View space (3D).
        _view[i] = _world[i] * viewMat;
        
        // View space (3D) -> Clip space (4D).
        _clip[i] = _view[i] * projectionMat;

        // TODO: fix clipping.

        //! Temporarily clip triangles by nuking them when one vertex is offscreen.
        if (!((-abs(_clip[i].w) <= _clip[i].x) && (_clip[i].x <= abs(_clip[i].w)) &&
              (-abs(_clip[i].w) <= _clip[i].y) && (_clip[i].y <= abs(_clip[i].w)) &&
              (-abs(_clip[i].w) <= _clip[i].z) && (_clip[i].z <= abs(_clip[i].w))))
            return false;
        
        // Clip space (4D) -> NDC (3D).
        _ndc[i] = _clip[i].toVector3(true);
        
        // NDC (3D) -> screen coords (2D).
        _screen[i] = ndcToScreenCoords(_ndc[i], viewport);
    }
    
    // Make sure the triangle vertices are in the right order to be drawn.
    swapTriangleVertices(_screen, _world, _view, _vertices);

    for (int i = 0; i < 3; i++)
    {
        // Bring uv coords to clip space.
        _perspectiveUV[i] = { _vertices[i].uv.x / _view[i].z, _vertices[i].uv.y / _view[i].z, 1 / _view[i].z };
    }

    return true;
}

static bool isTowardsCamera(const Vector3 _worldPos, const Vector3& _worldNormal, const Vector3& _camPos)
{
    return (_worldNormal & Vector3(_camPos, _worldPos)) <= 0;
}

void Renderer::drawTriangle(Triangle3 _triangle)
{
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

    // Back face culling.
    if (!isTowardsCamera(trianglePos, worldNormal, cameraPos))
        return;

    // Transform the triangle's vertices through the renderer's matrices.
    if (!transformVertices(3, &_triangle.a, localCoords, worldCoords, viewCoords, clipCoords, ndcCoords, screenCoords, perspectiveUV))
        return;

    // Draw triangle wireframe
    if (renderMode == RenderMode::WIREFRAME)
    {
        drawLine({ screenCoords[0], _triangle.a.normal, _triangle.a.color, _triangle.a.uv }, 
                 { screenCoords[1], _triangle.b.normal, _triangle.b.color, _triangle.b.uv });
        drawLine({ screenCoords[1], _triangle.b.normal, _triangle.b.color, _triangle.b.uv }, 
                 { screenCoords[2], _triangle.c.normal, _triangle.c.color, _triangle.c.uv });
        drawLine({ screenCoords[2], _triangle.c.normal, _triangle.c.color, _triangle.c.uv }, 
                 { screenCoords[0], _triangle.a.normal, _triangle.a.color, _triangle.a.uv });
        return;
    }

    // Compute Blinn-Phong lighting for each vertex / light.
    Color lightIntensity[3] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, };
    if (lightingMode == LightingMode::PHONG)
    {
        for (int i = 0; i < 3; i++)
        {
            if (renderMode == RenderMode::LIT)
                lightIntensity[i] = computePhong(lights, material, worldCoords[i].toVector3(), worldNormal, cameraPos);
            else 
                lightIntensity[i] = WHITE;
        }
    }

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
            // If p is on or inside all edges, render pixel.
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) 
            {
                // Make the barycentric coordinates percentages.
                float w0n = w0 / (float)(w0 + w1 + w2);
                float w1n = w1 / (float)(w0 + w1 + w2);
                float w2n = w2 / (float)(w0 + w1 + w2);

                // Compute the pixel depth.
                float depth = abs(perspectiveUV[0].z * w0n + perspectiveUV[1].z * w1n + perspectiveUV[2].z * w2n);
                      depth = 1 / depth;
                
                // Interpolate pixel lighting.
                Color pLight = WHITE;
                if (renderMode == RenderMode::LIT)
                {
                    if (lightingMode == LightingMode::PHONG)
                    {
                        pLight =
                        { 
                            w0n * lightIntensity[0].r + w1n * lightIntensity[1].r + w2n * lightIntensity[2].r,
                            w0n * lightIntensity[0].g + w1n * lightIntensity[1].g + w2n * lightIntensity[2].g,
                            w0n * lightIntensity[0].b + w1n * lightIntensity[1].b + w2n * lightIntensity[2].b,
                        };
                    }
                    else if (lightingMode == LightingMode::BLINN)
                    {
                        Vector3 pixelPos = (worldCoords[0] * w0n + worldCoords[1] * w1n + worldCoords[2] * w2n).toVector3();
                        pLight           = computePhong(lights, material, pixelPos, worldNormal, cameraPos);
                    }
                }

                // Define the pixel color.
                Color pCol { 0, 0, 0, 1 };

                if (texture.pixels == nullptr || texture.applyVertexColor)
                {
                    // Get the pixel color from barycentric coordinates.
                    pCol.r = _triangle.a.color.r * w0n + _triangle.b.color.r * w1n + _triangle.c.color.r * w2n; 
                    pCol.g = _triangle.a.color.g * w0n + _triangle.b.color.g * w1n + _triangle.c.color.g * w2n; 
                    pCol.b = _triangle.a.color.b * w0n + _triangle.b.color.b * w1n + _triangle.c.color.b * w2n; 
                    pCol.a = _triangle.a.color.a * w0n + _triangle.b.color.a * w1n + _triangle.c.color.a * w2n;
                }

                if (texture.pixels != nullptr)
                {
                    // Compute the uv coordinates.
                    Vector2 uv = { clamp(depth * (perspectiveUV[0].x * w0n + perspectiveUV[1].x * w1n + perspectiveUV[2].x * w2n), 0, 1),
                                   clamp(depth * (perspectiveUV[0].y * w0n + perspectiveUV[1].y * w1n + perspectiveUV[2].y * w2n), 0, 1) };

                    // Get the pixel color from the current texture.
                    Color texColor = texture.getPixelColor(floorInt(lerp(uv.x, 0, abs(texture.width ))), 
                                                           floorInt(lerp(uv.y, 0, abs(texture.height))),
                                                           pCol.a);

                    // Apply the texture color to the pixel color.
                    if (texture.applyVertexColor)
                    {
                        HSV pHSV = RGBtoHSV(texColor);
                        pCol = HSVtoRGB({ pCol.getHue(), pHSV.s, pHSV.v }, pCol.a);
                    }
                    else
                    {
                        pCol = texColor;
                    }
                }

                // Apply the pixel lighting to the pixel color.
                pCol *= pLight;

                // Draw the pixel.
                drawPixel(p.x, p.y, depth, pCol);
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

void Renderer::drawTriangles(Triangle3* _triangles, const unsigned int& _count)
{
    for (int i = 0; i < (int)_count; i++) drawTriangle(_triangles[i]);
}

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

void Renderer::drawDividedCube(const Color& _color, const float& _size, const float& _res)
{
    for (int x = 0; x < _res + 1; x++)
    {
        for (int y = 0; y < _res + 1; y++)
        {
            for (int z = 0; z < _res + 1; z++)
            {
                Vector3 curCoords = { - _size / 2 + (_size / _res) * x, 
                                      - _size / 2 + (_size / _res) * y, 
                                      - _size / 2 + (_size / _res) * z };

                // Left and right faces.
                if ((x == 0 || x == _res) && y < _res && z < _res)
                {
                    float xPos = (x == 0 ? -_size / 2 : _size / 2);

                    Triangle3 triangles[] = 
                    {
                        {
                            { { xPos, curCoords.y,                  curCoords.z                  }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(y  ) / _res } },
                            { { xPos, curCoords.y + (_size / _res), curCoords.z + (_size / _res) }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(y+1) / _res } },
                            { { xPos, curCoords.y + (_size / _res), curCoords.z                  }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(y+1) / _res } },
                        },
                        {
                            { { xPos, curCoords.y,                  curCoords.z                  }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(y  ) / _res } },
                            { { xPos, curCoords.y,                  curCoords.z + (_size / _res) }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(y  ) / _res } },
                            { { xPos, curCoords.y + (_size / _res), curCoords.z + (_size / _res) }, { (xPos > 0 ? 1.f : -1.f), 0, 0 }, _color, { (xPos>0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(y+1) / _res } },
                        },
                    };

                    drawTriangles(triangles, 2);
                }

                // Up and down faces.
                if ((y == 0 || y == _res) && x < _res && z < _res)
                {
                    float yPos = (y == 0 ? -_size / 2 : _size / 2);

                    Triangle3 triangles[] = 
                    {
                        {
                            { { curCoords.x,                  yPos, curCoords.z                  }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(x  ) / _res } },
                            { { curCoords.x + (_size / _res), yPos, curCoords.z + (_size / _res) }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(x+1) / _res } },
                            { { curCoords.x + (_size / _res), yPos, curCoords.z                  }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(x+1) / _res } },
                        },
                        {
                            { { curCoords.x,                  yPos, curCoords.z                  }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z  ) / _res : 1-(float)(z  ) / _res), (float)(x  ) / _res } },
                            { { curCoords.x,                  yPos, curCoords.z + (_size / _res) }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(x  ) / _res } },
                            { { curCoords.x + (_size / _res), yPos, curCoords.z + (_size / _res) }, { 0, (yPos > 0 ? 1.f : -1.f), 0 }, _color, { (yPos<0 ? (float)(z+1) / _res : 1-(float)(z+1) / _res), (float)(x+1) / _res } },
                        },
                    };

                    drawTriangles(triangles, 2);
                }

                // Front and back faces.
                if ((z == 0 || z == _res) && x < _res && y < _res)
                {
                    float zPos = (z == 0 ? -_size / 2 : _size / 2);

                    Triangle3 triangles[] = 
                    {
                        {
                            { { curCoords.x,                  curCoords.y,                  zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x  ) / _res : 1-(float)(x  ) / _res), (float)(y  ) / _res } },
                            { { curCoords.x + (_size / _res), curCoords.y + (_size / _res), zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x+1) / _res : 1-(float)(x+1) / _res), (float)(y+1) / _res } },
                            { { curCoords.x + (_size / _res), curCoords.y,                  zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x+1) / _res : 1-(float)(x+1) / _res), (float)(y  ) / _res } },
                        },
                        {
                            { { curCoords.x,                  curCoords.y,                  zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x  ) / _res : 1-(float)(x  ) / _res), (float)(y  ) / _res } },
                            { { curCoords.x,                  curCoords.y + (_size / _res), zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x  ) / _res : 1-(float)(x  ) / _res), (float)(y+1) / _res } },
                            { { curCoords.x + (_size / _res), curCoords.y + (_size / _res), zPos }, { 0, 0, (zPos > 0 ? 1.f : -1.f) }, _color, { (zPos<0 ? (float)(x+1) / _res : 1-(float)(x+1) / _res), (float)(y+1) / _res } },
                        },
                    };

                    drawTriangles(triangles, 2);
                }
            }
        }
    }
}

void Renderer::drawSphere(const Color& _color, const float& _r, const int& _lon, const int& _lat)
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

// --- Material and texture setters --- //

void Renderer::setTexture (const TextureData& _textureData) { texture  = _textureData; }
void Renderer::setMaterial(const Material& _material)       { material = _material;    }

// ---------- Miscellaneous ---------- //

void Renderer::showImGuiControls()
{
    // Render mode static.
    static const char* rModeItems[]{ "Unlit", "Lit", "Wireframe", "Z-Buffer" };
    static int rModeCur = 1;

    // Lighting mode static.
    static const char* lModeItems[]{ "Phong", "Blinn" };
    static int lModeCur = 0;
    
    // Displaying components.
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);

    ImGui::Combo("Render Mode", &rModeCur, rModeItems, IM_ARRAYSIZE(rModeItems));
    renderMode = (RenderMode)rModeCur;

    ImGui::Combo("Lighting Mode", &lModeCur, lModeItems, IM_ARRAYSIZE(lModeItems));
    lightingMode = (LightingMode)lModeCur;

    ImGui::Checkbox("Apply vertex colors to texture", &texture.applyVertexColor);
}
