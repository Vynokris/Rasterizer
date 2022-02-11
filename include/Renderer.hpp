#pragma once

#include <Framebuffer.hpp>
#include <Camera.hpp>

struct Viewport
{
    Viewport(const int _x, const int _y, unsigned int _width, unsigned int _height)
             : x(_x), y(_y), width(_width), height(_height) {}

    int x, y;
    unsigned int width, height;
};

enum class ViewMode : int
{
    DEFAULT,
    WIREFRAME,
    ZBUFFER,
};

class Renderer
{
private:
    Viewport viewport;
    Color lineColor = { 1.f, 1.f, 1.f, 1.f };

    std::vector<Mat4> modelMat;
    Mat4 viewMat;
    Mat4 projectionMat;

    ViewMode currentView = ViewMode::DEFAULT;

public:
    Framebuffer framebuffer;
    
    Renderer(const unsigned int& width, const unsigned int& height);
    ~Renderer();

    // -- Setters for the three matrices -- //

    void setModel(const Mat4& modelMatrix);
    void setView(const Mat4& viewMatrix);
    void setProjection(const Mat4& projectionMatrix);

    // ------- Model transformations ------ //

    void modelPushMat  ();
    void modelPopMat   ();
    void modelTranslate(const float& x, const float& y, const float& z);
    void modelRotateX  (const float& angle);
    void modelRotateY  (const float& angle);
    void modelRotateZ  (const float& angle);
    void modelScale    (const float& scaleX, const float& scaleY, const float& scaleZ);

    // --------- Drawing functions -------- //

    void setTexture(float* colors32Bits, const unsigned int& width, const unsigned int& height);
    void drawPixel(const unsigned int& x, const unsigned int& y, const float& depth, const Color& color);
    void drawLine(Vector3 p0, Vector3 p1, const Color& color);
    void drawTriangles(geometry3D::Triangle3* triangles, const unsigned int& count, const geometry3D::Frustum& frustum);
    void drawDividedQuad(const geometry3D::Frustum& frustum, const Color& color, const float& size = 1.f, const bool& negateNormals = false);
    void drawCube(const geometry3D::Frustum& frustum, const Color& color, const float& size = 1.f);
    void drawSphere(const geometry3D::Frustum& frustum, const float& r, const int& lon, const int& lat, const Color& color);

    // --- View mode getters / setters --- //

    ViewMode getViewMode() const;
    void     setViewMode(const ViewMode& mode);

    // ---------- Miscellaneous ---------- //
    
    void showImGuiControls();

private:
    void drawTriangle(geometry3D::Triangle3 triangle, const geometry3D::Frustum& frustum, bool wasClipped = false);
};


