#pragma once
#include <vector>
#include <Camera.hpp>
#include <Renderer.hpp>

class Scene
{
private:
    double time = 0.0;
    std::vector<Vertex> vertices;
    float scale = 1.f;
    
public:
    Scene();
    ~Scene();
    void update(float p_deltaTime, Renderer& p_renderer, const Camera& camera);
    void showImGuiControls();
};
