#include <imgui.h>
#include <my_math.hpp>

#include "Scene.hpp"

using namespace geometry3D;

Scene::Scene()
{
    // HERE: Load the scene
    // Setup some vertices to test
    vertices.clear();

    // FIRST TRIANGLE          |        pos        |  |      normal      |  |         color          |  |     uv     |
    vertices.push_back(Vertex{ { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ {  0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ {  0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
    
    // SECOND TRIANGLE         |        pos        |  |      normal      |  |         color          |  |     uv     |
    vertices.push_back(Vertex{ { -1.0f,-0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ { 1.0f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ { 1.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
}

Scene::~Scene()
{
    // HERE: Unload the scene
    vertices.clear();
}

void Scene::update(float deltaTime, Renderer& renderer, const Camera& camera)
{
    // Set the matrices.
    renderer.setModel(getTransformMatrix({ 0, 0, 0 }, { 0, 0, 0 }, { scale, scale, scale }));
    renderer.setView(camera.getViewMatrix());
    renderer.setProjection(camera.getProjection());

    // Draw the first triangle.
    //renderer.modelPushMat();
    //renderer.modelTranslate(0, 0, -4);
    //renderer.drawTriangles(vertices.data(), 3, camera.getFrustum());
    //renderer.modelPopMat();

    // Draw the second triangle.
    //renderer.modelTranslate(0, 0, -2);
    //renderer.drawTriangles(vertices.data() + 3, 3, camera.getFrustum());

    // Draw Sphere.
    renderer.modelPushMat();
    renderer.modelTranslate(0, 0, -3);
    renderer.drawSphere(camera.getFrustum(), 1, 16, 16);
    renderer.modelPopMat();
    
    time += deltaTime;
}

void Scene::showImGuiControls()
{
    // TODO: Object manager.
}
