
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
    m_pitch = 0;
    m_yaw = 0;
    m_speed = 0.f;
}

//* ---------- MAIN CAMERA METHODS ---------- *//

void Camera::update(const float deltaTime, const CameraInputs& inputs)
{
    // Rotate camera first.
    setRotation(m_pitch + inputs.deltaX / 200.f, 
                m_yaw   - inputs.deltaY / 200.f);

    // Then move the camera.
    m_speed = deltaTime * m_acceleration;
    if (inputs.moveForward)  setPosition({ m_pos.x, m_pos.y, m_pos.z + m_speed });
    if (inputs.moveBackward) setPosition({ m_pos.x, m_pos.y, m_pos.z - m_speed });
    if (inputs.moveLeft)     setPosition({ m_pos.x - m_speed, m_pos.y, m_pos.z });
    if (inputs.moveRight)    setPosition({ m_pos.x + m_speed, m_pos.y, m_pos.z });
}

//* -------- CAMERA GETTERS METHODS --------- *//

Mat4 Camera::getWorldTransform() const { return getViewMatrix().inv4(); }
float Camera::getPitch()         const { return m_pitch; }
float Camera::getYaw()           const { return m_pitch; }

Mat4 Camera::getProjection() const
{
    float yScale = 1.f / tan(m_fov / 2.f);
    float xScale = yScale / m_aspect;

    return Mat4(
        xScale, 0,      0,                                 0,
        0,      yScale, 0,                                 0,
        0,      0,      m_far          / (m_far - m_near), 1,
        0,      0,    - m_far * m_near / (m_far - m_near), 0
    );
}

Mat4 Camera::getViewMatrix() const
{
    Mat4 output(true);
    return output * render3D::getTransformMatrix(m_pos, { m_yaw, m_pitch, 0 }, { 1, 1, 1 }, true);
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
    ImGui::Text("Position: %.2f, %.2f, %.2f", m_pos.x, m_pos.y, m_pos.z);
    ImGui::Text("Pitch: %.2f, Yaw = %.2f", m_pitch, m_yaw);
    ImGui::Text("Speed: %.2f", m_speed);
}