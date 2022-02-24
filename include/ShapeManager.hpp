#pragma once

#include <vector>
#include <string>
#include <imgui_impl_glfw.h>

#include <my_math.hpp>
#include <Light.hpp>
#include <Texture.hpp>
#include <Renderer.hpp>

#define MAX_SHAPES 10

enum class ShapeTypes : int { TRIANGLE, QUAD, CUBE, DIVIDED_CUBE, SPHERE };

struct Shape
{
    ShapeTypes type;

    float   size;
    Vector3 pos, rot;
    
    Material material;
    int      textureID = 0;
    bool applyVertexColor = false;

    // Used only for shapes that are not triangles.
    Color color = WHITE;

    // Used only for divided cube and sphere.
    int subdivisions = 6;

    // Used only for shapes that are triangles.
    Triangle3 triangleData = Triangle3();
};

class ShapeManager
{
private:
    std::vector<std::string> textureNames;
    std::vector<TextureData> textures;
    std::vector<Shape> shapes;

public:
    ShapeManager();
    ~ShapeManager();

    void   addShape   (Shape _shape = { ShapeTypes::CUBE, 1, { 0, 0, 2 }, { 0, 0, 0 }, { 1, 1, 0.2 } });
    Shape& getShape   (const int& _index);
    int    getShapeNum();
    void   delShape   (const int& _index);

    void drawShapes(Renderer& _renderer);

    void showImGuiControls();
};
