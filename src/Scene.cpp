#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>
#include <Scene.hpp>

using namespace geometry3D;

Scene::Scene()
{
    // Setup some vertices to test
    triangles.clear();
    lights.clear();

    // FIRST TRIANGLE      |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
                          { {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
                          { {  0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.5f, 0.5f } } });
    
    // SECOND TRIANGLE     |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -1.0f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
                          { { 1.0f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { { 1.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } } });
    
    // LIGHT 1
    lights.push_back((LightParams){ { 2, 2, 2 }, Vector3(), ColorToVec3(WHITE), ColorToVec3(WHITE), ColorToVec3(WHITE) });
}

Scene::~Scene()
{
    triangles.clear();
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

    // Draw the first triangle.
    _renderer.modelPushMat();
    _renderer.modelTranslate(0, 0, 4);
    _renderer.drawTriangles(&triangles[0], 1);
    _renderer.modelPopMat();

    // Draw the second triangle.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0, 0, 2);
    // _renderer.drawTriangles(&triangles[1], 1);
    // _renderer.modelPushMat();

    // Draw scene components.
    Material mat = { { 0.0215, 0.1745, 0.0215 }, { 0.07568, 0.61424, 0.07568 },	{ 0.633, 0.727811, 0.633 },	0.6, 1 };
    _renderer.modelPushMat();
    _renderer.modelTranslate(-0.5, 0, 2);
    _renderer.drawCube(RED);
    _renderer.modelPopMat();

    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0.5, 0, 2);
    // _renderer.drawSphere(1, 8, 8, BLUE);
    // _renderer.modelPopMat();
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

std::vector<Light> Scene::getLights() const { return lights; }

void Scene::showImGuiControls() { }
