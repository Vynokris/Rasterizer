#pragma once
#include <vector>
#include <Camera.hpp>
#include <Renderer.hpp>

class Scene
{
private:
    double time = 0.f, deltaTime = 0.f;
    std::vector<Triangle3> triangles;
    float scale = 1.f;
    
public:
    Scene();
    ~Scene();
    void update(const float& _deltaTime, Renderer& _renderer, const Camera& _camera);
    void showImGuiControls();
};
