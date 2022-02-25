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
    // Setup default lights.
    lights.clear();
    lights.push_back({ 5, 1, 0.2, 0.1, {  2,  1.5,  2 }, RED   });
    lights.push_back({ 5, 1, 0.2, 0.1, {  2, -2,   -2 }, GREEN });
    lights.push_back({ 5, 1, 0.2, 0.1, { -1,  1.5, -2 }, BLUE  });
    lights.push_back({ 5, 1, 0.2, 0.1, {  2,  0,    4 }, WHITE });

    // Setup default shapes.
    shapeManager.addShape({ ShapeTypes::CUBE,   1, { -2, 0, 2 }, { 0, 0, 0 }, { 1, 1, 0.2 } });
    shapeManager.addShape({ ShapeTypes::SPHERE, 1, {  2, 0, 2 }, { 0, 0, 0 }, { 1, 1, 0.2 } });
    shapeManager.getShape(1).subdivisions = 10;
}

void Scene::update(const float& _deltaTime, Renderer& _renderer, const Camera& _camera)
{
    // Update time variables.
    deltaTime  = _deltaTime;
    time      +=  deltaTime;

    // Set the world matrix.
    _renderer.setModel(getTransformMatrix({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }));

    // Set the view matrix.
    switch (_camera.getViewMode())
    {
        case ViewMode::FIRST_PERSON: _renderer.setView(_camera.getViewMat());   break;
        case ViewMode::THIRD_PERSON: _renderer.setView(_camera.getLookAtMat()); break;
        default: break;
    }

    // Set the projection matrix.
    _renderer.setProjection(_camera.getPerspective());

    // Draw all of the scene's shapes.
    shapeManager.drawShapes(_renderer);
}

vector<Light>* Scene::getLights() { return &lights; }

void Scene::showImGuiControls(Renderer& _renderer)
{    
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

                ImGui::DragFloat3("Position", &lights[i].pos.x, 0.25);
                ImGui::ColorEdit4("Color",    &lights[i].color.r);

                ImGui::PushItemWidth(100);
                ImGui::InputFloat("Light range",           &lights[i].range);
                ImGui::InputFloat("Constant attenuation",  &lights[i].constantAttenuation);
                ImGui::InputFloat("Linear attenuation",    &lights[i].linearAttenuation);
                ImGui::InputFloat("Quadratic attenuation", &lights[i].quadraticAttenuation);
                ImGui::PopItemWidth();

                ImGui::EndGroup();
            }
            ImGui::PopID();
        }
    }

    // Shapes pannel.
    shapeManager.showImGuiControls();
}
