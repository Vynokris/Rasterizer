
#include <imgui.h>

#include <my_math.hpp>

#include <Camera.hpp>

using namespace arithmetic;
using namespace matrix;

Camera::Camera(const unsigned int width, const unsigned int height,
               const float fov, const float near, const float far,
               const float acceleration)
      : m_width(width), m_height(height),
        m_fov(fov), m_near(near), m_far(far),
        m_acceleration(acceleration)
{
    m_aspect = m_width / m_height;
    m_pitch = m_yaw = m_speed = 0.f;
}

//* ---------- MAIN CAMERA METHODS ---------- *//

void Camera::update(const float deltaTime, const CameraInputs& inputs)
{
    // Rotate camera first.
    setRotation(inputs.deltaY / 3.f, inputs.deltaX / 3.f);

    // Then move the camera.
    m_speed = deltaTime * m_acceleration;
    if (inputs.moveForward)  setPosition({m_pos.x, m_pos.y, m_pos.z + m_speed});
    if (inputs.moveBackward) setPosition({m_pos.x, m_pos.y, m_pos.z - m_speed});
}

//* -------- CAMERA GETTERS METHODS --------- *//

Mat4 Camera::getWorldTransform() const { return getViewMatrix().inv4(); }
float Camera::getPitch()         const { return m_pitch; }
float Camera::getYaw()           const { return m_pitch; }

Mat4 Camera::getProjection() const
{
    float yScale = 1.f / tanf(degToRad(m_fov / 2.f));
    float xScale = yScale / m_aspect;

    return Mat4(
        xScale, 0,      0,                                  0,
        0,      yScale, 0,                                  0,
        0,      0,    - m_far          / (m_far - m_near), -1,
        0,      0,    - m_far * m_near / (m_far - m_near),  0
    );
}

Mat4 Camera::getViewMatrix() const
{
    Vector2 pitchAngle = { cosf(m_pitch), sinf(m_pitch)};
    Vector2 yawAngle   = { cosf(m_yaw),   sinf(m_yaw)};

    // Compute axis according to the camera pitch and yaw.
    Vector3 xaxis = { yawAngle.x, 0, -yawAngle.y};
    Vector3 yaxis = { yawAngle.y * pitchAngle.y,  pitchAngle.x, yawAngle.x * pitchAngle.y};
    Vector3 zaxis = { yawAngle.y * pitchAngle.x, -pitchAngle.y, yawAngle.x * pitchAngle.x};

    // Compute view matrix.
    return Mat4(
        xaxis.z,          yaxis.z,          zaxis.z,        0.f,
        xaxis.x,          yaxis.x,          zaxis.x,        0.f,
        xaxis.y,          yaxis.y,          zaxis.z,        0.f,
      -(xaxis & m_pos), -(yaxis & m_pos), -(zaxis & m_pos), 1.f
    );
}

//* -------- CAMERA SETTERS METHODS --------- *//

void Camera::setPosition(const Vector3& pos)
{
    m_pos = pos;
}

void Camera::setRotation(const float pitch, const float yaw)
{
    m_pitch = pitch; m_yaw = yaw;
}

//* ------------------ MISC. ---------------- *//

void Camera::showImGuiControls()
{
    //TODO
}