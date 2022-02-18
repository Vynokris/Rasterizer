#include <vector>

#include <Mesh.hpp>

#include <my_math.hpp>

using namespace std;
using namespace geometry3D;

// -------- Mesh constructors -------- //

Mesh::Mesh(const std::vector<Vertex>& _vertices ,
           const std::vector<int>&    _indices)
    : vertices(_vertices), indices(_indices)
{ }

Mesh::~Mesh()
{
    /*
    if (texture != nullptr)
        delete[] texture->pixels;*/
    vertices.clear();
    indices.clear();
}

// ------------ Mesh getters ----------- //

      Mat4&        Mesh::getTransform()                     { return transform;        }
const Material&    Mesh::getMaterial()                const { return *material;        }
      TextureData* Mesh::getTexture()                       { return texture;          }
const Vertex&      Mesh::getVertex(const int& _index) const { return vertices[_index]; }
const vector<int>& Mesh::getIndices()                 const { return indices;          }

// ----------- Mesh settters ---------- //

void Mesh::setVisibility (const bool&        _visibility) { isHidden = _visibility;                       }
void Mesh::setColor      (const Color&       _color   )   { for (Vertex it : vertices) it.color = _color; }
void Mesh::setMaterial   (      Material*    _material)   { material = _material;                         }
void Mesh::setTexture    (      TextureData* _texture )   { texture  = _texture;                          }

void Mesh::appendVertices(const std::vector<Vertex>& _vertices)
{
    vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
}

void Mesh::appendTriangle(const int& index0, const int& index1, const int& index2)
{
    indices.push_back(index0);
    indices.push_back(index1);
    indices.push_back(index2);
}

// ------ Primary shapes meshes ------ //

vector<Vertex> createQuad(const float& _size, const bool& _negateNormals)
{
    // Create quad triangle pair.
    vector<Vertex> output =
    {
        { {  _size / 2, -_size / 2, 0 }, (_negateNormals ? Vector3( -1, 1, -1 ) : Vector3( 1, -1, 1  )), WHITE, { 0, 0 } },
        { { -_size / 2, -_size / 2, 0 }, (_negateNormals ? Vector3( 1, 1, 1   ) : Vector3( -1, -1, -1)), WHITE, { 1, 0 } },
        { {  _size / 2,  _size / 2, 0 }, (_negateNormals ? Vector3( -1, -1, -1) : Vector3( 1, 1, 1   )), WHITE, { 0, 1 } },
        { { -_size / 2,  _size / 2, 0 }, (_negateNormals ? Vector3( 1, -1, 1  ) : Vector3( -1, 1, -1 )), WHITE, { 1, 1 } },
    };

    return output;
}

Mesh createCube(const float& _size)
{
    Mesh cube;
    vector<Mat4> transformMat;
    transformMat.push_back(Mat4(true));

    // Build the cube.
    for (int i = 0; i < 6; i++)
    {
        if      (i < 4)  transformMat.back() *= getYRotationMatrix(PI / 2);
        else if (i == 4) transformMat.back() *= getXRotationMatrix(PI / 2);
        else if (i == 5) transformMat.back() *= getXRotationMatrix(PI);

        transformMat.push_back(transformMat.back());
        transformMat.back() *= getTranslationMatrix({0, 0, _size / 2});

        // Create the vertices for one face and transform them.
        vector<Vertex> quad = createQuad(_size);
        for (int j = 0; j < (int)quad.size(); j++)
        {
            quad[j].pos    = (Vector4(quad[j].pos,    1) * transformMat.back()).toVector3();
            quad[j].normal = (Vector4(quad[j].normal, 0) * transformMat.back()).toVector3().getNormalized();
        }

        // Insert the vertices into the vectors.
        cube.appendVertices(quad);
        cube.appendTriangle(i*6, i*6+1, i*6+2);
        cube.appendTriangle(i*6, i*6+2, i*6+3);

        transformMat.pop_back();
    }
    return cube;
}

Mesh createSphere(const float& _lon, const float& _lat, const float& _r)
{
    // TODO: fix this. Vertices are created 4 times oops.
    Mesh output;

    // Build the sphere.
    for (int j = 0; j < _lat; j += 2)
    {
        float theta0 = ( (j + 0) / (float)_lat) * PI;
        float theta1 = ( (j + 1) / (float)_lat) * PI;

        for (int i = 0; i < _lon; i += 2)
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
            std::vector<Vertex> vertices = 
            {
                { c0, normal, WHITE, { 0, 0 } },
                { c1, normal, WHITE, { 0, 1 } },
                { c2, normal, WHITE, { 1, 0 } },
                { c3, normal, WHITE, { 0, 1 } }
            };

            output.appendVertices(vertices);
            output.appendTriangle(i*6, i*6+1, i*6+2);
            output.appendTriangle(i*6, i*6+2, i*6+3);
        }
    }

    return output;
}
