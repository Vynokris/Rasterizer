#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>
#include <Mesh.hpp>
#include <Scene.hpp>

using namespace std;
using namespace geometry3D;

Scene::Scene()
{
    // triangles.clear();
    meshes.clear();
    lights.clear();

    /*
    // Setup some vertices to test
    // FIRST TRIANGLE       |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } } });
    
    // SECOND TRIANGLE      |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, {  0.0f, 0.0f } },
                          { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, 1.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f, 1.0f } } });
    */
    
    // Meshes.
    meshes.push_back(createCube(1));
    meshes.push_back(createSphere(10, 10, 1));

    // Load and bind a texture to the cube.
    TextureData baseTexture = loadBmpData("art/UrAppreciated.bmp");
    meshes[0].setTexture(&baseTexture);

    // Lights.
    lights.push_back((LightParams){ { 2, 2, 2 }, Vector3(), ColorToVec3(WHITE), ColorToVec3(WHITE), ColorToVec3(WHITE) });
}

Scene::~Scene()
{
    meshes.clear();
    lights.clear();
}

void Scene::update(const float& _deltaTime, Renderer& _renderer, const Camera& _camera)
{
    // Set the matrices.
    _renderer.setModel(getTransformMatrix({ 0, 0, 0 }, { 0, 0, 0 }, { scale, scale, scale }));
    switch (_camera.getViewMode())
    {
    case ViewMode::FIRST_PERSON:
        _renderer.setView(_camera.getViewMat());
        break;
    case ViewMode::THIRD_PERSON:
        _renderer.setView(_camera.getLookAtMat({ 0, 0, 2 }));
        break;
    default: break;
    }
    _renderer.setProjection(_camera.getPerspective());

    for (Mesh mesh : meshes)
    {
        _renderer.modelPushMat();
        _renderer.getModel() *= mesh.getTransform();
        for (int i = 0; i < (int)mesh.getIndices().size(); i += 3)
        {
            _renderer.drawTriangle({ mesh.getVertex(mesh.getIndices()[i]), 
                                     mesh.getVertex(mesh.getIndices()[i+1]), 
                                     mesh.getVertex(mesh.getIndices()[i+2]) },
                                     mesh.getTexture());
        }
        _renderer.modelPopMat();
    }

    // Draw the first triangle.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0, 0, 3);
    // _renderer.drawTriangles(&triangles[0], 1);
    // _renderer.modelPopMat();

    // Draw the second triangle.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0, 0, 3);
    // _renderer.drawTriangles(&triangles[1], 1);
    // _renderer.modelPopMat();

    // Draw cube.
    // Material mat = { { 0.0215, 0.1745, 0.0215 }, { 0.07568, 0.61424, 0.07568 },	{ 0.633, 0.727811, 0.633 },	0.6, 1 };
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(-0.5, 0, 2);
    // _renderer.modelRotateX(fmod(time/2, 2*PI));
    // _renderer.modelRotateY(fmod(time/2, 2*PI));
    // _renderer.modelRotateZ(fmod(time/2, 2*PI));
    // _renderer.drawCube(MAGENTA);
    // _renderer.modelPopMat();

    // Draw sphere.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0.5, 0, 2);
    // _renderer.modelRotateX(fmod(time/2, 2*PI));
    // _renderer.modelRotateY(fmod(time/2, 2*PI));
    // _renderer.modelRotateZ(fmod(time/2, 2*PI));
    // _renderer.drawSphere(1, 10, 10, CYAN);
    // _renderer.modelPopMat();
    
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

vector<Light>& Scene::getLights() { return lights; }

void Scene::showImGuiControls() { }
