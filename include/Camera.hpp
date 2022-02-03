#pragma once

#include <types.hpp>

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
    Camera(const uint p_width, const uint p_height);

    void Update(const float p_deltaTime, const CameraInputs& p_inputs);
    mat4x4 GetViewMatrix();
    mat4x4 GetProjection();

    void ShowImGuiControls();
};