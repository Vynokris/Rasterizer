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
    lights.push_back({ 5, 1, 0.2, 0.1, {  0,  1.5, 2 }, RED   });
    lights.push_back({ 5, 1, 0.2, 0.1, {  2,  0,   2 }, GREEN });
    lights.push_back({ 5, 1, 0.2, 0.1, { -3, -1.5, 0 }, WHITE  });
    lights.push_back({ 5, 1, 0.2, 0.1, {  0,  0,   4 }, BLUE });

    // Setup default shapes.
    shapeManager.addShape({ ShapeTypes::CUBE,   1, { -0.5, 0, 2 }, { 0, 0, 0 }, { 1, 1, 0.2 } });
    shapeManager.addShape({ ShapeTypes::SPHERE, 1, {  0.5, 0, 2 }, { 0, 0, 0 }, { 1, 1, 0.2 } });
    shapeManager.getShape(0).color = { 1, 1, 1, 0.7 };
    shapeManager.getShape(1).subdivisions = 20;
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
        // Compute items padding.
        ImVec2 p0 = ImGui::GetCursorScreenPos();
        ImGui::SetCursorScreenPos({ p0.x+7, p0.y });
        ImGui::BeginGroup();

        // Light instatiation.
        if (ImGui::Button("Create a new light", ImGui::GetItemRectSize()) && (int)lights.size() < MAX_LIGHTS) 
            lights.push_back({ 5, 1, 0.2, 0.1, Vector3(), WHITE });
        ImGui::EndGroup();
            
        // Compute light info padding.
        ImGui::SetCursorScreenPos({ p0.x+10, p0.y+23 });
        ImGui::BeginGroup();
        
        // Lights parameters.
        for (int i = 0; i < (int)lights.size(); i++)
        {
            string lightName = "Light " + to_string(i);
            ImGui::PushID(MAX_SHAPES + i);

            // Draw header with items inside.
            if (ImGui::CollapsingHeader(lightName.c_str()))
            {
                ImVec2 p1 = ImGui::GetCursorScreenPos();
                ImGui::SetCursorScreenPos({ p1.x+7, p1.y });
                ImGui::BeginGroup();

                // Remove light button.
                if(ImGui::Button("Remove") && (int)lights.size() > 0) 
                    lights.erase(lights.begin() + i);

                // Position and color.
                ImGui::DragFloat3("Position", &lights[i].pos.x, 0.25);
                ImGui::ColorEdit4("Color",    &lights[i].color.r);

                // Light attributes.
                ImGui::PushItemWidth(100);
                ImGui::DragFloat("Light range",           &lights[i].range               , 0.05, 0, 20);
                ImGui::DragFloat("Constant attenuation",  &lights[i].constantAttenuation , 0.05, 0, 5);
                ImGui::DragFloat("Linear attenuation",    &lights[i].linearAttenuation   , 0.05, 0, 5);
                ImGui::DragFloat("Quadratic attenuation", &lights[i].quadraticAttenuation, 0.05, 0, 5);
                ImGui::PopItemWidth();

                ImGui::EndGroup();
            }
            ImGui::PopID();
        }
        ImGui::EndGroup();
    }

    // Shapes pannel.
    shapeManager.showImGuiControls();
}
