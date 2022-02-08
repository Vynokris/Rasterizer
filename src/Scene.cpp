#include <imgui.h>
#include <my_math.hpp>

#include "Scene.hpp"

using namespace render3D;

Scene::Scene()
{
    // HERE: Load the scene
    // Setup some vertices to test
    vertices.clear();
    //                          |       pos        |      normal     |           color           |    uv    |
    vertices.push_back(rdrVertex{-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.0f, 0.0f });
    vertices.push_back(rdrVertex{ 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f }, 0.0f, 0.0f });
    vertices.push_back(rdrVertex{ 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, { 1.0f, 0.0f, 1.0f, 1.0f }, 0.0f, 0.0f });
}

Scene::~Scene()
{
    // HERE: Unload the scene
    vertices.clear();
}

void Scene::update(float deltaTime, Renderer& renderer, const Camera& camera)
{
    //(float)cos(time) * 0.5f, (float)sin(time) * 0.1f, 0.f, 1.f);
    Mat4 matrix = getTransformMatrix({ 0, 0, -2 }, { 0, 0, 0 }, { scale, scale, scale });
    renderer.setModel(matrix);
    renderer.setView(camera.getViewMatrix());
    renderer.setProjection(camera.getProjection());

    // Draw
    // renderer.drawPixel(0, 0, { 1, 1, 1, 1 });
    // renderer.drawLine({ 600, 400, 0 }, { 50, 50, 0 }, { 1, 1, 1, 1 });
    renderer.drawTriangles(vertices.data(), (int)vertices.size());

    time += deltaTime;
}

void Scene::showImGuiControls()
{
    ImGui::SliderFloat("scale", &scale, 0.f, 10.f);

    ImGui::Text("\nPoint 0:");
    ImGui::SliderFloat3("position 0", &vertices[0].x, -2, 2);
    ImGui::ColorEdit4  ("color 0",    &vertices[0].color.r);
    ImGui::Text("\nPoint 1:");
    ImGui::SliderFloat3("position 1", &vertices[1].x, -2, 2);
    ImGui::ColorEdit4  ("color 1", &vertices[1].color.r);
    ImGui::Text("\nPoint 2:");
    ImGui::SliderFloat3("position 2", &vertices[2].x, -2, 2);
    ImGui::ColorEdit4  ("color 2", &vertices[2].color.r);
}
