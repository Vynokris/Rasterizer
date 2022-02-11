#include <imgui.h>
#include <my_math.hpp>

#include "Scene.hpp"

using namespace geometry3D;

Scene::Scene()
{
    // HERE: Load the scene
    // Setup some vertices to test
    triangles.clear();

    // FIRST TRIANGLE      |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } } });
    
    // SECOND TRIANGLE     |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -1.0f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
                          { { 1.0f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { { 1.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } } });
}

Scene::~Scene()
{
    // HERE: Unload the scene
    triangles.clear();
}

void Scene::update(const float& _deltaTime, Renderer& renderer, const Camera& camera)
{
    // Set the matrices.
    renderer.setModel(getTransformMatrix({ 0, 0, 0 }, { 0, 0, 0 }, { scale, scale, scale }));
    renderer.setView(camera.getViewMatrix());
    renderer.setProjection(camera.getProjection());

    // Draw the first triangle.
    renderer.modelPushMat();
    renderer.modelTranslate(0, 0, -4);
    renderer.drawTriangles(&triangles[0], 1, camera.getFrustum());
    renderer.modelPopMat();

    // Draw the second triangle.
    //renderer.modelTranslate(0, 0, -2);
    //renderer.drawTriangles(&triangles[1], 1, camera.getFrustum());

    // Draw scene components.
    renderer.modelPushMat();

    renderer.modelTranslate(-0.5, 0, 2);
    renderer.drawCube(camera.getFrustum(), RED);

    renderer.modelTranslate(0.5, 0, 2);
    renderer.drawSphere(camera.getFrustum(), 1, 8, 8, BLUE);

    renderer.modelPopMat();
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

void Scene::showImGuiControls()
{
}
