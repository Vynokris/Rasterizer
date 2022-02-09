#include <cstdio>
#include <cstring>
#include <cassert>

#include <imgui.h>
#include <my_math.hpp>
using namespace arithmetic;
using namespace geometry3D;

#include "Renderer.hpp"

Renderer::Renderer(const unsigned int& width, const unsigned int& height)
    : viewport(0, 0, width, height)
    , framebuffer(width, height)
{}

Renderer::~Renderer()
{}

// -- Setters for the three matrices -- //

void Renderer::setModel(const Mat4& modelMatrix)           { modelMat.clear(); modelMat.push_back(modelMatrix); }
void Renderer::setView(const Mat4& viewMatrix)             { viewMat = viewMatrix;                              }
void Renderer::setProjection(const Mat4& projectionMatrix) { projectionMat = projectionMatrix;                  }

// ------- Model transformations ------ //

void Renderer::modelPushMat  ()                                                              { modelMat.push_back(modelMat.back()); }
void Renderer::modelPopMat   ()                                                              { if (modelMat.size() > 1) modelMat.pop_back(); }
void Renderer::modelTranslate(const float& x, const float& y, const float& z)                { modelMat.back() = modelMat.back() * getTranslationMatrix({ x, y, z });          }
void Renderer::modelRotateX  (const float& angle)                                            { modelMat.back() = modelMat.back() * getXRotationMatrix(angle);                  }
void Renderer::modelRotateY  (const float& angle)                                            { modelMat.back() = modelMat.back() * getYRotationMatrix(angle);                  }
void Renderer::modelRotateZ  (const float& angle)                                            { modelMat.back() = modelMat.back() * getZRotationMatrix(angle);                  }
void Renderer::modelScale    (const float& scaleX, const float& scaleY, const float& scaleZ) { modelMat.back() = modelMat.back() * getScaleMatrix({ scaleX, scaleY, scaleZ }); }

// --------- Drawing functions -------- //

void Renderer::setTexture(float* colors32Bits, const unsigned int& width, const unsigned int& height)
{
    // TODO
}

void Renderer::drawPixel(const unsigned int& x, const unsigned int& y, const float& depth, const Color& color)
{
    if (framebuffer.depthBuffer[y * framebuffer.getWidth() + x] >= depth)
    {
        framebuffer.depthBuffer[y * framebuffer.getWidth() + x] = depth;
        switch (getViewMode())
        {
        case ViewMode::DEFAULT:
        case ViewMode::WIREFRAME: framebuffer.colorBuffer[y * framebuffer.getWidth() + x] = color;                      break;
        case ViewMode::ZBUFFER:   framebuffer.colorBuffer[y * framebuffer.getWidth() + x] = { depth, depth, depth, 1 }; break;
        default: break;
        }
    }
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
            drawPixel(p0.x, p0.y, 0, color);
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

static Vector3 ndcToScreenCoords(const Vector3& ndc, const Viewport& viewport)
{
    return {
        ndc.x * viewport.width  + viewport.width  / 2, 
        ndc.y * viewport.height + viewport.height / 2, 
        ndc.z
    };
}

static int barycentricCoords(const Vector3& a, const Vector3& b, const Vector3& c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

void Renderer::drawTriangle(Vertex* vertices, const Frustum& frustum, bool wasClipped)
{
    // Store triangle vertices positions.
    Vector3 localCoords[3] = {
        { vertices[0].pos.x, vertices[0].pos.y, vertices[0].pos.z },
        { vertices[1].pos.x, vertices[1].pos.y, vertices[1].pos.z },
        { vertices[2].pos.x, vertices[2].pos.y, vertices[2].pos.z },
    };
    
    // Local space (3D) -> World space (3D).
    Vector4 worldCoords[3] = {
        { (Vector4{ localCoords[0], 1 } * modelMat.back()) },
        { (Vector4{ localCoords[1], 1 } * modelMat.back()) },
        { (Vector4{ localCoords[2], 1 } * modelMat.back()) },
    };
    
    // World space (3D) -> View space (3D).
    Vector4 viewCoords[3] = {
        { (worldCoords[0] * viewMat) },
        { (worldCoords[1] * viewMat) },
        { (worldCoords[2] * viewMat) },
    };

    // Clip the triangle against the frustum.
    /*
    if (!wasClipped)
    {
        Triangle3 viewTriangle(
            { viewCoords[0].toVector3(), vertices[0].normal, vertices[0].color, vertices[0].uv }, 
            { viewCoords[1].toVector3(), vertices[1].normal, vertices[1].color, vertices[1].uv }, 
            { viewCoords[2].toVector3(), vertices[2].normal, vertices[2].color, vertices[2].uv }
        );
        std::vector<Triangle3> clippedTriangles = clipTriangleWithFrustum(viewTriangle, frustum);
        
        for (int i = 0; i < (int)clippedTriangles.size(); i++)
            drawTriangle(&clippedTriangles[0].a, frustum, true);
        return;
    }
    */
    
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
        { clipCoords[2].getHomogenized().toVector3().getNormalized() }
    };
    
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
    if (getViewMode() == ViewMode::WIREFRAME)
    {
        drawLine(screenCoords[0], screenCoords[1], lineColor);
        drawLine(screenCoords[1], screenCoords[2], lineColor);
        drawLine(screenCoords[2], screenCoords[0], lineColor);
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
            Color pCol = { vertices[0].color.r * w0n + vertices[1].color.r * w1n + vertices[2].color.r * w2n, 
                           vertices[0].color.g * w0n + vertices[1].color.g * w1n + vertices[2].color.g * w2n, 
                           vertices[0].color.b * w0n + vertices[1].color.b * w1n + vertices[2].color.b * w2n, 
                           vertices[0].color.a * w0n + vertices[1].color.a * w1n + vertices[2].color.a * w2n };

            // Compute depth
            float depth = screenCoords[0].z * w0n + screenCoords[1].z * w1n + screenCoords[2].z * w2n;

            // If p is on or inside all edges, render pixel.
            if ((w0 | w1 | w2) >= 0) drawPixel(p.x, p.y, 1 / clipCoords[2].z * lerp(depth, -1, 1), pCol);

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

void Renderer::drawTriangles(Vertex* vertices, const unsigned int& count, const Frustum& frustum)
{
    // Calculate mvp from matrices
    // Transform vertex list to triangles into colorBuffer
    for (int i = 0; i < (int)count; i += 3) drawTriangle(&vertices[i], frustum);
}


void Renderer::drawDividedQuad(const float& size, const Frustum& frustum, const bool& negateNormals)
{

    Vertex v[] = {
        { { -size / 2,  size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, V2_ZERO  },
        { { -size / 2, -size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, { 0, 1 } },
        { {  size / 2,  size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, { 1, 0 } },

        { {  size / 2, -size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, V2_ONE   },
        { {  size / 2,  size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, { 1, 0 } },
        { { -size / 2, -size / 2, 0 }, { 0, 0, (negateNormals ? 1 : -1) }, WHITE, { 0, 1}  },
    };
    
    drawTriangles(v, 0, frustum);
    drawTriangles(v, 3, frustum);
}

void Renderer::drawCube(const Cube& cube, const Frustum& frustum)
{
    modelPushMat();

    modelTranslate(0, 0, cube.getSize() / 2);

    // Render the 4 side faces.
    for (int i = 0; i < 4; i++)
    {
        glRotatef(-90, 0, 1, 0);
        modelRotateY(-PI/2);
        glTranslatef(-cube.getSize()/2, 0, cube.getSize()/2);
        drawDividedQuad(cube.getSize(), frustum, true);
    }

    // Render the upper and lower faces.
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -cube.getSize()/2, -cube.getSize()/2);
    drawDividedQuad(cube.getSize(), frustum, true);
    glTranslatef(0, 0, cube.getSize());
    drawDividedQuad(cube.getSize(), frustum);

    modelPopMat();
}

// --- View mode getters / setters --- //

ViewMode Renderer::getViewMode() const
{
    return currentView;
}

void Renderer::setViewMode(const ViewMode& mode)
{
    currentView = mode;
}

// ---------- Miscellaneous ---------- //

void Renderer::showImGuiControls()
{
    // Static variables.
    static const char* items[]{"Default", "Wireframe", "Z-Buffer"};
    static int curItem = 0;
    
    // Displaying components.
    ImGui::ColorEdit4("BG Color", &framebuffer.clearColor.r);

    ImGui::ListBox("View Mode", &curItem, items, IM_ARRAYSIZE(items));
    setViewMode((ViewMode)curItem);

    ImGui::Text("\nMatrices:");
    ImGui::Text(("Model:\n"      + modelMat.back().printStr(false)).c_str());
    ImGui::Text(("View:\n"       + viewMat.printStr        (false)).c_str());
    ImGui::Text(("Projection:\n" + projectionMat.printStr  (false)).c_str());
}
