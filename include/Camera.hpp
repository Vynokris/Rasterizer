#pragma once

#include <my_math.hpp>

struct Frustum
{
    geometry3D::Plane3 up;
    geometry3D::Plane3 down;
    geometry3D::Plane3 left;
    geometry3D::Plane3 right;
    geometry3D::Plane3 near;
    geometry3D::Plane3 far;
};

struct CameraInputs
{
    float deltaX, deltaY;
    bool moveForward, moveBackward, moveLeft, moveRight;
    // ... and more if needed
};

class Camera
{
private:
    unsigned int m_width, m_height;
    float m_fov, m_aspect, m_near, m_far,
          m_pitch, m_yaw, m_speed, m_acceleration;
    Vector3 m_pos, m_direction;
    Frustum m_frustum;

public:
    Camera(const unsigned int width, const unsigned int height,
           const float fov, const float near, const float far,
           const float acceleration);

    // Main methods.
    void update(const float deltaTime, const CameraInputs& inputs);

    // Getters.
    Mat4         getWorldTransform() const;
    float        getPitch()          const;
    float        getYaw()            const;
    matrix::Mat4 getProjection()     const;
    matrix::Mat4 getViewMatrix()     const;

    // Setters.
    void setPosition(const Vector3& pos);
    void setRotation(const float pitch, const float yaw);

    // Misc.
    void showImGuiControls();
};