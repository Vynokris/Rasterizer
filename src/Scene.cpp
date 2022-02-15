#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>
#include <Scene.hpp>

using namespace geometry3D;

Scene::Scene()
{
    // HERE: Load the scene
    // Setup some vertices to test
    triangles.clear();
    lights.clear();

    // FIRST TRIANGLE      |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } } });
    
    // SECOND TRIANGLE     |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, {  0.0f, 0.0f } },
                          { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, 1.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f, 1.0f } } });
}

Scene::~Scene()
{
    // HERE: Unload the scene
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
    _renderer.modelTranslate(0, 0, 2);
    _renderer.drawTriangles(&triangles[0], 1);
    _renderer.modelPopMat();

    // Draw the second triangle.
    _renderer.modelPushMat();
    _renderer.modelTranslate(0, 0, 2);
    _renderer.drawTriangles(&triangles[1], 1);
    _renderer.modelPushMat();

    // Draw scene components.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(-0.5, 0, 2);
    // _renderer.drawCube(RED);
    // _renderer.modelPopMat();

    // _renderer.modelPushMat();
    // _renderer.modelTranslate(0.5, 0, 2);
    // _renderer.drawSphere(1, 8, 8, BLUE);
    // _renderer.modelPopMat();
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

void Scene::showImGuiControls()
{
}
