#pragma once

#include <Renderer.hpp>
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
    
    Renderer(const unsigned int& _width, const unsigned int& _height);
    ~Renderer();

    // -- Setters for the three matrices -- //

    void setModel(const Mat4& _modelMatrix);
    void setView(const Mat4& _viewMatrix);
    void setProjection(const Mat4& _projectionMatrix);

    // ------- Model transformations ------ //

    void modelPushMat  ();
    void modelPopMat   ();
    void modelTranslate(const float& _x, const float& _y, const float& _z);
    void modelRotateX  (const float& _angle);
    void modelRotateY  (const float& _angle);
    void modelRotateZ  (const float& _angle);
    void modelScale    (const float& _scaleX, const float& _scaleY, const float& _scaleZ);

    // --------- Drawing functions -------- //

    void setTexture(float* _colors32Bits, const int& _width, const int& _height);
    void drawPixel(const unsigned int& _x, const unsigned int& _y, const float& _depth, const Color& _color);
    void drawLine(const geometry3D::Vertex& _p0, const geometry3D::Vertex& _p1);
    void drawTriangles(geometry3D::Triangle3* _triangles, const unsigned int& _count);
    void drawDividedQuad(const Color& _color, const float& _size = 1.f, const bool& _negateNormals = false);
    void drawCube(const Color& _color, const float& _size = 1.f);
    void drawSphere(const float& _r, const int& _lon, const int& _lat, const Color& _color);

    // --- View mode getters / setters --- //

    ViewMode getViewMode() const;
    void     setViewMode(const ViewMode& _mode);

    // ---------- Miscellaneous ---------- //
    
    void showImGuiControls();

private:
    void drawTriangle(geometry3D::Triangle3 _triangle);
};


