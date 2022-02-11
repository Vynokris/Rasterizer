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

void Scene::update(float deltaTime, Renderer& renderer, const Camera& camera)
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

    // Draw Cube.
    // renderer.modelPushMat();
    // renderer.modelTranslate(0, 0, -5);
    // renderer.drawCube(camera.getFrustum(), 1);
    // renderer.modelPopMat();

    // Draw Sphere.
    // renderer.modelPushMat();
    // renderer.modelTranslate(0, 0, -3);
    // renderer.drawSphere(camera.getFrustum(), 1, 16, 16);
    // renderer.modelPopMat();
    
    time += deltaTime;
}

void Scene::showImGuiControls()
{
    // TODO: Object manager.
}
