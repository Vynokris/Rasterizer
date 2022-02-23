#pragma once

#include <vector>

#include <Light.hpp>
#include <Camera.hpp>
#include <Renderer.hpp>

class Renderer;

class Scene
{
private:
    double time = 0.f, deltaTime = 0.f;
    std::vector<Triangle3> triangles;
    std::vector<Light> lights;
    float scale = 1.f;
    
public:
    Scene();
    ~Scene();

    void update(const float& _deltaTime, Renderer& _renderer, const Camera& _camera);
    std::vector<Light>* getLights();
    void showImGuiControls();
};
