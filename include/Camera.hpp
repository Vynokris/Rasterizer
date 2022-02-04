#pragma once

#include <my_math.hpp>

struct CameraInputs
{
    float deltaX;
    float deltaY;
    bool moveForward;
    bool moveBackward;
    // ... and more if needed
};

class Camera
{
public:
    Camera(const unsigned int p_width, const unsigned int p_height);

    void update(const float p_deltaTime, const CameraInputs& p_inputs);
    matrix::Mat4 getViewMatrix();
    matrix::Mat4 getProjection();

    void showImGuiControls();
};