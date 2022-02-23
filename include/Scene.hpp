#pragma once

#include <vector>

#include <Light.hpp>
#include <Camera.hpp>
#include <Renderer.hpp>

class Renderer;

class Scene
{
private:
    double time = 0, deltaTime = 0;
    std::vector<Light> lights;
    
public:
    Scene();
    ~Scene() {}

    void update(const float& _deltaTime, Renderer& _renderer, const Camera& _camera);

    std::vector<Light>* getLights();

    void showImGuiControls(Renderer& _renderer);
};
