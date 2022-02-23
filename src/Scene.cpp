#include <string>

#include <imgui.h>
#include <my_math.hpp>

#include <Light.hpp>
#include <Scene.hpp>

#define MAX_LIGHTS 10

using namespace std;
using namespace geometry3D;

Scene::Scene()
{
    // Setup some vertices to test
    triangles.clear();
    lights.clear();

    // FIRST TRIANGLE       |        pos        |  |      normal      |  |         color          |  |     uv     |
    triangles.push_back({ { { -0.5f,-0.5f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                          { {  0.2f,-0.6f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
                          { {  1.0f,-0.3f, 0.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } } });
    
    // LIGHT 1
    lights.push_back({ 5, 1, 0.2, 0.1, {  2,  1.5,  2 }, RED   });
    lights.push_back({ 5, 1, 0.2, 0.1, {  2, -2,   -2 }, GREEN });
    lights.push_back({ 5, 1, 0.2, 0.1, { -1,  1.5, -2 }, BLUE  });
    lights.push_back({ 5, 1, 0.2, 0.1, {  2,  0,    4 }, WHITE });
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
        case ViewMode::FIRST_PERSON: _renderer.setView(_camera.getViewMat());              break;
        case ViewMode::THIRD_PERSON: _renderer.setView(_camera.getLookAtMat({ 0, 0, 2 })); break;
        default: break;
    }
    _renderer.setProjection(_camera.getPerspective());

    // Draw a cube on each light.
    for (int i = 0; i < (int)lights.size(); i++)
    {
        _renderer.modelPushMat();
        _renderer.modelTranslate(lights[i].pos.x, lights[i].pos.y - 0.2, lights[i].pos.z);
        _renderer.drawCube(WHITE, 0.1);
        _renderer.modelPopMat();
    }

    // Draw a plane.
    _renderer.modelPushMat();
    _renderer.modelTranslate(-2, 2, 2);
    _renderer.modelRotateX(PI/2);
    _renderer.drawDividedQuad(WHITE, 2, true);
    _renderer.modelTranslate(2, 0, 0);
    _renderer.drawDividedQuad(WHITE, 2, true);
    _renderer.modelTranslate(2, 0, 0);
    _renderer.drawDividedQuad(WHITE, 2, true);
    _renderer.modelPopMat();

    // Draw a trangle.
    _renderer.modelPushMat();
    _renderer.modelTranslate(0, 0, 2);
    _renderer.drawTriangles(triangles.data(), 1);
    _renderer.modelPopMat();

    // Draw a cube.
    _renderer.modelPushMat();
    _renderer.modelTranslate(-2, 0, 2);
    // _renderer.modelRotateX(fmod(time/2, 2*PI));
    // _renderer.modelRotateY(fmod(time/2, 2*PI));
    // _renderer.modelRotateZ(fmod(time/2, 2*PI));
    // _renderer.drawCube({ 1, 1, 1, 1 });
    _renderer.drawDividedCube({ 1, 1, 1, 1 }, 1, 10);
    _renderer.modelPopMat();

    // Draw a sphere.
    // _renderer.modelPushMat();
    // _renderer.modelTranslate(2, 0, 2);
    // _renderer.modelRotateX(-fmod(time/2, 2*PI));
    // _renderer.modelRotateY(-fmod(time/2, 2*PI));
    // _renderer.modelRotateZ(-fmod(time/2, 2*PI));
    // _renderer.drawSphere({ 1, 1, 1, 1 }, 0.7, 32, 32);
    // _renderer.modelPopMat();
    
    
    deltaTime = _deltaTime;
    time      += deltaTime;
}

std::vector<Light>* Scene::getLights() { return &lights; }

void Scene::showImGuiControls(Renderer& _renderer)
{
    // Material pannel.
    if (ImGui::CollapsingHeader("Material"))
    {
        static Material output = _renderer.getMaterial();
        ImGui::SliderFloat("Diffuse",  &output.diffuse,  0, 10);
        ImGui::SliderFloat("Specular", &output.specular, 0, 10);
        ImGui::SliderFloat("Shiness",  &output.shiness,  0, 10);
        _renderer.setMaterial(output);
    }
    
    // Light pannel.
    if (ImGui::CollapsingHeader("Lights"))
    {
        // Light instatiation.
        if (ImGui::Button("Add", { 50, 18 }) && (int)lights.size() < MAX_LIGHTS) lights.push_back({ 5, 1, 0.2, 0.1, Vector3(), WHITE });
                
        // Lights parameters.
        for (int i = 0; i < (int)lights.size(); i++)
        {
            string lightName = "Light ";
            lightName += to_string(i);

            ImGui::PushID(i);
            if(ImGui::Button("Remove") && (int)lights.size() > 0) lights.erase(lights.begin() + i);

            // Compute lights items padding.
            ImVec2 p0      = ImGui::GetCursorScreenPos();
            ImVec2 offset  = ImGui::GetItemRectSize();
            ImVec2 padding = { p0.x + offset.x + 5, p0.y };

            // Draw header with items inside.
            ImGui::SameLine();
            if (ImGui::CollapsingHeader(lightName.c_str()))
            {
                ImGui::SetCursorScreenPos(padding);
                ImGui::BeginGroup();

                ImGui::SliderFloat3("Position", &lights[i].pos.x, -10, 10);
                ImGui::ColorEdit4("Color",      &lights[i].color.r);

                ImGui::PushItemWidth(100);
                ImGui::InputFloat("Light range",  &lights[i].range);
                ImGui::InputFloat("Constant attenuation", &lights[i].constantAttenuation);
                ImGui::InputFloat("Linear attenuation", &lights[i].linearAttenuation);
                ImGui::InputFloat("Quadratic attenuation", &lights[i].quadraticAttenuation);
                ImGui::PopItemWidth();

                ImGui::EndGroup();
            }
            ImGui::PopID();
        }
    }
}
