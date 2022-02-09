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
    vertices.push_back(Vertex{ { -1.0f,-0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ { 1.0f, -0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
    vertices.push_back(Vertex{ { 1.0f,  0.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
}

Scene::~Scene()
{
    // HERE: Unload the scene
    vertices.clear();
}

void Scene::update(float deltaTime, Renderer& renderer, const Camera& camera)
{
    renderer.setModel(getTransformMatrix({ 0, 0, -4 }, { 0, 0, 0 }, { scale, scale, scale }));
    renderer.setView(camera.getViewMatrix());
    renderer.setProjection(camera.getProjection());

    // Draw
    renderer.drawTriangles(vertices.data(), (int)vertices.size(), camera.getFrustum());

    time += deltaTime;
}

void Scene::showImGuiControls()
{
    // TODO: Object manager.
}
