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

    // FIRST TRIANGLE       |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } } });
    
    // SECOND TRIANGLE      |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, {  0.0f, 0.0f } },
                          { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, 1.0f } },
                          { {  0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f, 1.0f } } });
    
    // LIGHT 1
    lights.push_back({ {  2,  2,  2 }, Vector3(), RED   });
    lights.push_back({ {  2, -2, -2 }, Vector3(), GREEN });
    lights.push_back({ { -2,  2, -2 }, Vector3(), BLUE  });
    lights.push_back({ {  0, -1,  4 }, Vector3(), WHITE });
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
    _renderer.modelPushMat();
    _renderer.modelTranslate(0, 0, 2);
    _renderer.modelRotateX(fmod(time/2, 2*PI));
    _renderer.modelRotateY(fmod(time/2, 2*PI));
    _renderer.modelRotateZ(fmod(time/2, 2*PI));
    // _renderer.drawCube({ 1, 1, 1, 1 });
    _renderer.drawDividedCube({ 1, 1, 1, 0.5 }, 1, 10);
    _renderer.modelPopMat();

    // Draw sphere.
    _renderer.modelPushMat();
    _renderer.modelTranslate(0, 0, 2);
    _renderer.modelRotateX(-fmod(time/2, 2*PI));
    _renderer.modelRotateY(-fmod(time/2, 2*PI));
    _renderer.modelRotateZ(-fmod(time/2, 2*PI));
    _renderer.drawSphere({ 1, 1, 1, 1 }, 0.7, 6, 6);
    _renderer.modelPopMat();
    
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

std::vector<Light> Scene::getLights() const { return lights; }

void Scene::showImGuiControls() { }
