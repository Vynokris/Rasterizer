#pragma once

#include <vector>

#include <Renderer.hpp>
#include <Framebuffer.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <BmpLoader.hpp>

struct Viewport
{
    Viewport(const int _x, const int _y, unsigned int _width, unsigned int _height)
             : x(_x), y(_y), width(_width), height(_height) {}

    int x, y;
    unsigned int width, height;
};

enum class RenderMode   : int { UNLIT, LIT, WIREFRAME, ZBUFFER };

enum class LightingMode : int { PHONG, BLINN };   

class Renderer
{
private:
    // TODO: Make object class and apply texture and material.
    TextureData texture;
    Material    material; 
    Viewport    viewport;

    std::vector<Mat4> modelMat;
    Mat4 viewMat;
    Mat4 projectionMat;

    // Scene components copied datas.
    std::vector<Light>* lights;

    RenderMode   renderMode;
    LightingMode lightingMode;

public:
    Framebuffer framebuffer;
    
    Renderer(const unsigned int& _width, const unsigned int& _height, std::vector<Light>* _lights);

    // -- Setters for the three matrices -- //

    void setModel     (const Mat4& _modelMatrix);
    void setView      (const Mat4& _viewMatrix);
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

    void drawPixel        (const unsigned int& _x, const unsigned int& _y, const float& _depth, Color _color);
    void drawLine         (const geometry3D::Vertex& _p0, const geometry3D::Vertex& _p1);
    bool transformVertices(int _count, Vertex* _vertices, Vector3* _local, Vector4* _world, Vector4* _view, Vector4* _clip, Vector3* _ndc, Vector3* _screen, Vector3* _perspectiveUV);
    void drawTriangles    (geometry3D::Triangle3* _triangles, const unsigned int& _count);
    void drawDividedQuad  (const Color& _color, const float& _size = 1.f, const bool& _negateNormals = false);
    void drawCube         (const Color& _color, const float& _size = 1.f);
    void drawDividedCube  (const Color& _color, const float& _size = 1.f, const float& _res = 1.f);
    void drawSphere       (const Color& _color, const float& _r, const int& _lon, const int& _lat);

    // --- Material and texture setters --- //

    void setTexture (const TextureData& _textureData);
    void setMaterial(const Material& _material);

    // ---------- Miscellaneous ---------- //
    
    void showImGuiControls();

private:
    void drawTriangle(geometry3D::Triangle3 _triangle);
};
