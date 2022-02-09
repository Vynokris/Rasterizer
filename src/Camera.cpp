
#include <imgui.h>
#include <my_math.hpp>
#include <Camera.hpp>

using namespace arithmetic;
using namespace matrix;
using namespace geometry3D;

Camera::Camera(const unsigned int width, const unsigned int height,
               const float fov, const float near, const float far,
               const float acceleration)
      : m_width(width), m_height(height),
        m_fov(fov), m_near(near), m_far(far),
        m_acceleration(acceleration)
{
    m_aspect = m_width / m_height;
    m_pitch = m_yaw = m_speed = 0.f;

    // Setup frustum distances.
    m_frustum.near.distance  = near;
    m_frustum.far.distance   = far;
    m_frustum.up.distance    = (float)height / 2;
    m_frustum.down.distance  = (float)height / 2;
    m_frustum.left.distance  = (float)width  / 2;
    m_frustum.right.distance = (float)width  / 2;
}

//* ---------- MAIN CAMERA METHODS ---------- *//

void Camera::update(const float deltaTime, const CameraInputs& inputs)
{
    // Rotate camera first (Yaw locked between -90° and 90°).
    setRotation(m_pitch + inputs.deltaX / 180.f, 
                clamp(m_yaw - inputs.deltaY / 180.f, -PI/2, PI/2));
    
    // Compute and update the camera frustum normals.
    Vector3 fwdVec   = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch - PI/2);
    Vector3 downVec  = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI,   2*PI - m_pitch - PI/2);
    Vector3 rightVec = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch);
    m_frustum.near.normal   = fwdVec;
    m_frustum.far.normal    = fwdVec.getNegated();
    m_frustum.up.normal     = downVec;
    m_frustum.down.normal   = downVec.getNegated();
    m_frustum.left.normal   = rightVec;
    m_frustum.right.normal  = rightVec.getNegated();

    ImGui::Begin("Debug info");
    {
        ImGui::Text("Near  normal: (%.2f, %.2f, %.2f)",   m_frustum.near.normal.x,  m_frustum.near.normal.y,  m_frustum.near.normal.z );
        ImGui::Text("Far   normal: (%.2f, %.2f, %.2f)",   m_frustum.far.normal.x,   m_frustum.far.normal.y,   m_frustum.far.normal.z  );
        ImGui::Text("Up    normal: (%.2f, %.2f, %.2f)",   m_frustum.up.normal.x,    m_frustum.up.normal.y,    m_frustum.up.normal.z   );
        ImGui::Text("Down  normal: (%.2f, %.2f, %.2f)",   m_frustum.down.normal.x,  m_frustum.down.normal.y,  m_frustum.down.normal.z );
        ImGui::Text("Left  normal: (%.2f, %.2f, %.2f)",   m_frustum.left.normal.x,  m_frustum.left.normal.y,  m_frustum.left.normal.z );
        ImGui::Text("Right normal: (%.2f, %.2f, %.2f)\n", m_frustum.right.normal.x, m_frustum.right.normal.y, m_frustum.right.normal.z);
    }
    ImGui::End();

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
    return output * getTransformMatrix(m_pos, { m_yaw, m_pitch, 0 }, { 1, 1, 1 }, true);
}

Frustum Camera::getFrustum() const
{
    return m_frustum;
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
    ImGui::Text("Pitch: %.2f° | Yaw = %.2f°", radToDeg(m_pitch), radToDeg(m_yaw));
    ImGui::Text("Speed: %.2f", m_speed);
}