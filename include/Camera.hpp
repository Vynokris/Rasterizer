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
private:
    Vector3 m_eye;
    unsigned int m_width, m_height;
    float m_fov, m_aspect, m_near, m_far, m_pitch, m_yaw;

public:
    Camera(const unsigned int width, const unsigned int height,
           const float fov, const float near, const float far);

    // Main methods.
    void update(const float deltaTime, const CameraInputs& inputs);
    matrix::Mat4 getProjection();
    Mat4         getWorldTransform();

    // FP camera methods.
    Vector2      getFPRotation();
    matrix::Mat4 getFPViewMatrix();

    // Look at camera methods.
    matrix::Mat4 getLookAtMatrix(Vector3 m_at, Vector3 m_up);

    // Misc.
    void showImGuiControls();
};