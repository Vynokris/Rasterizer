#include <imgui.h>
#include <my_math.hpp>

#include "Scene.hpp"

Scene::Scene()
{
    // HERE: Load the scene
    // Setup some vertices to test
    vertices = {
    //  |       pos        |      normal     |               color                 |    uv     |
        {-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 131.f/255, 144.f/255, 250.f/255, 1.0f, 0.0f, 0.0f },
        { 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 250.f/255, 199.f/255, 72.0f/255, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 248.f/255, 141.f/255, 173.f/255, 1.0f, 0.0f, 0.0f },
    };
}

Scene::~Scene()
{
    // HERE: Unload the scene
}

void Scene::update(float deltaTime, Renderer& renderer)
{
    // HERE: Update (if needed) and display the scene

    // Hard coded matrix
    // TODO: Remove this and use proper functions !
    Mat4 matrix(scale, 0.f, 0.f, 0.f,
                0.f, scale, 0.f, 0.f,
                0.f, 0.f, scale, 0.f,
                (float)cos(time) * 0.5f, (float)sin(time) * 0.1f, 0.f, 1.f);
    renderer.setModel(matrix);

    // Draw
    // renderer.drawPixel(0, 0, { 1, 1, 1, 1 });
    // renderer.drawLine({ 600, 400, 0 }, { 50, 50, 0 }, { 1, 1, 1, 1 });
    renderer.drawTriangles(vertices.data(), (int)vertices.size());

    time += deltaTime;
}

void Scene::showImGuiControls()
{
    ImGui::SliderFloat("scale", &scale, 0.f, 10.f);
    ImGui::SliderFloat2("point 0", &vertices[0].x, 0, 800);
    ImGui::SliderFloat2("point 1", &vertices[1].x, 0, 800);
    ImGui::SliderFloat2("point 2", &vertices[2].x, 0, 800);
}
