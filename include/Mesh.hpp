#pragma once

#include <vector>

#include <BmpLoader.hpp>
#include <my_math.hpp>

// Material structure.
struct Material
{
    Color ambient, diffuse, specular;
    float shiness, attenuation;
};

// Mesh class.
class Mesh
{
private:
    bool isHidden;
    
    Mat4         transform;
    Material*    material = nullptr;
    TextureData* texture  = nullptr;

    std::vector<Vertex> vertices;
    std::vector<int>    indices;

public:
    // Constructors / destrucors.
    Mesh() {}
    
    Mesh(const std::vector<Vertex>& _vertices,
         const std::vector<int>&    _indices);

    ~Mesh();

    // Main methods.
    void draw();

    // Getters.
          Mat4&                getTransform()                    ;
    const Material&            getMaterial()                const;
          TextureData*         getTexture()                      ;
    const Vertex&              getVertex(const int& _index) const;
    const int&                 getIndex(const int& _index)  const;
    const int                  getVerticesNumber()          const;
    const int                  getIndicesNumber()           const;

    // Setters.
    void setVisibility (const bool&                _visibility)                    ;
    void setColor      (const Color&               _color     )                    ;
    void setMaterial   (      Material*            _material  )                    ;
    void setTexture    (      TextureData*         _texture   )                    ;
    void appendVertices(const std::vector<Vertex>& _vertices  )                    ;
    void appendTriangle(const int& _index0, const int& _index1, const int& _index2);
};

// Primary shapes meshes.
std::vector<Vertex> createQuad(const float& _size, const bool& _negateNormals = false);

Mesh createCube(const float& _size = 1.f);

Mesh createSphere(const float& _lon, const float& _lat, const float& _r = 1.f);