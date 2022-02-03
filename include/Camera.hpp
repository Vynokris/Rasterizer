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

    void Update(const float p_deltaTime, const CameraInputs& p_inputs);
    matrix::Mat4 GetViewMatrix();
    matrix::Mat4 GetProjection();

    void ShowImGuiControls();
};