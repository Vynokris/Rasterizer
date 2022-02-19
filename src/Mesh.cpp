#include <vector>

#include <Mesh.hpp>

#include <my_math.hpp>

using namespace std;
using namespace geometry3D;

// -------- Mesh constructors -------- //

Mesh::Mesh(const vector<Vertex>& _vertices ,
           const vector<int>&    _indices)
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

      Mat4&                Mesh::getTransform()                      { return transform;             }
const Material&            Mesh::getMaterial()                 const { return *material;             }
      TextureData*         Mesh::getTexture()                        { return texture;               }
const Vertex&              Mesh::getVertex(const  int& _index) const { return vertices[_index];      }
const int&                 Mesh::getIndex(const  int& _index)  const { return indices[_index];       }
const int                  Mesh::getVerticesNumber()           const { return (int)vertices.size();  };
const int                  Mesh::getIndicesNumber()            const { return (int)indices.size();   };

// ----------- Mesh settters ---------- //

void Mesh::setVisibility (const bool&        _visibility) { isHidden = _visibility;                       }
void Mesh::setColor      (const Color&       _color   )   { for (Vertex it : vertices) it.color = _color; }
void Mesh::setMaterial   (      Material*    _material)   { material = _material;                         }
void Mesh::setTexture    (      TextureData* _texture )   { texture  = _texture;                          }

//! DEBUG PURPOSE ONLY ====================================================================================
static void debugVertices(vector<Vertex> _vertices, const char* _infoStatus)
{
    for (Vertex it : _vertices)
        printf("+ VERTEX %s: %.2f, %.2f, %.2f | %.2f, %.2f, %.2f | %.2f, %.2f\n", _infoStatus,
                it.pos.x, it.pos.y, it.pos.z, it.normal.x, it.normal.y, it.normal.z, it.uv.x, it.uv.y);
}
//! DEBUG PURPOSE ONLY ====================================================================================

void Mesh::appendVertices(const vector<Vertex>& _vertices)
{
    debugVertices(_vertices, "APPENDED"); //! DEBUG

    vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
}

void Mesh::appendTriangle(const int& _index0, const int& _index1, const int& _index2)
{
    printf("> TRIANGLE APPENDED: %d %d %d\n", _index0, _index1, _index2); //! DEBUG
    
    indices.push_back(_index0);
    indices.push_back(_index1);
    indices.push_back(_index2);
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

    printf("# QUAD CREATED\n"); //! DEBUG

    return output;
}

Mesh createCube(const float& _size)
{
    Mesh cube;
    vector<Mat4> transformMat;
    transformMat.push_back(Mat4(true));

    printf("\n\n-------------- CUBE CREATION --------------\n"); //! DEBUG

    // Build the cube.
    for (int i = 0; i < 6; i++)
    {
        printf("\n| FACE %d\n", i + 1); //! DEBUG
        
        if      (i < 4)  transformMat.back() *= getYRotationMatrix(PI / 2);
        else if (i == 4) transformMat.back() *= getXRotationMatrix(PI / 2);
        else if (i == 5) transformMat.back() *= getXRotationMatrix(PI);

        transformMat.push_back(transformMat.back());
        transformMat.back() *= getTranslationMatrix({0, 0, _size / 2});

        // Create the vertices for one face and transform each of them.
        vector<Vertex> quad = createQuad(_size);

        for (int j = 0; j < (int)quad.size(); j++)
        {
            quad[j].pos    = (Vector4(quad[j].pos,    1) * transformMat.back()).toVector3();
            quad[j].normal = (Vector4(quad[j].normal, 0) * transformMat.back()).toVector3().getNormalized();
        }
    

        // Insert the vertices into the vectors.
        cube.appendVertices(quad);
        cube.appendTriangle(i * 6, i * 6 + 1, i * 6 + 2);
        cube.appendTriangle(i * 6, i * 6 + 2, i * 6 + 3);

        transformMat.pop_back();
    }
    printf("\n-------------- TOTAL VERTICES: %d --------------", cube.getVerticesNumber()); //! DEBUG
    return cube;
}

Mesh createSphere(const float& _lon, const float& _lat, const float& _r)
{
    // TODO: fix this. Vertices are created 4 times oops.
    Mesh sphere;

    printf("\n\n-------------- SPHERE CREATION --------------n\n"); //! DEBUG

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
            vector<Vertex> vertices = 
            {
                { c0, normal, WHITE, { 0, 0 } },
                { c1, normal, WHITE, { 0, 1 } },
                { c2, normal, WHITE, { 1, 0 } },
                { c3, normal, WHITE, { 0, 1 } }
            };

            debugVertices(vertices, "CREATED"); //! DEBUG

            sphere.appendVertices(vertices);
            sphere.appendTriangle(i * 6, i * 6 + 1, i * 6 + 2);
            sphere.appendTriangle(i * 6, i * 6 + 2, i * 6 + 3);
        }
    }

    printf("\n-------------- TOTAL VERTICES: %d --------------", sphere.getVerticesNumber()); //! DEBUG
    return sphere;
}
