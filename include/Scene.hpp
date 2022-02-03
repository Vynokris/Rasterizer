#pragma once
#include <vector>
#include <Renderer.hpp>

class Scene
{
private:
    double time = 0.0;
    std::vector<rdrVertex> vertices;
    float scale = 1.f;
    
public:
    Scene();
    ~Scene();
    void Update(float p_deltaTime, Renderer& p_renderer);

private:
    void ShowImGuiControls();

};
