
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

    /*
    // Setup frustum distances.
    m_frustum.near.distance  = near;
    m_frustum.far.distance   = far;
    m_frustum.up.distance    = (float)height;
    m_frustum.down.distance  = (float)height;
    m_frustum.left.distance  = (float)width;
    m_frustum.right.distance = (float)width;
    printf("Frustum distances: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", m_frustum.near.distance, m_frustum.far.distance, m_frustum.up.distance, m_frustum.down.distance, m_frustum.left.distance, m_frustum.right.distance);
    
    // Compute and update the camera frustum normals.
    Vector3 fwdVec   = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch - PI/2);
    Vector3 downVec  = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI,   2*PI - m_pitch - PI/2);
    Vector3 rightVec = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch);
    m_frustum.near.normal   = fwdVec               ;
    m_frustum.far.normal    = fwdVec.getNegated()  ;
    m_frustum.up.normal     = downVec              ;
    m_frustum.down.normal   = downVec.getNegated() ;
    m_frustum.left.normal   = rightVec             ;
    m_frustum.right.normal  = rightVec.getNegated();
    */
}

//* ---------- MAIN CAMERA METHODS ---------- *//

void Camera::update(const float deltaTime, const CameraInputs& inputs)
{
    // Rotate camera (Yaw locked between -90° and 90°, Pitch reset to 0 when it reach 2PI).
    setRotation(fmodf(m_pitch + inputs.deltaX / 180.f, 2*PI), 
                clamp(m_yaw   - inputs.deltaY / 180.f, -PI/2, PI/2));
    
    /*
    // Compute and update the camera frustum normals.
    Vector3 fwdVec   = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch - PI/2);
    Vector3 downVec  = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI,   2*PI - m_pitch - PI/2);
    Vector3 rightVec = geometry3D::getSphericalCoords(1, 2*PI - m_yaw + PI/2, 2*PI - m_pitch);
    m_frustum.near.normal   = fwdVec               ;
    m_frustum.far.normal    = fwdVec.getNegated()  ;
    m_frustum.up.normal     = downVec              ;
    m_frustum.down.normal   = downVec.getNegated() ;
    m_frustum.left.normal   = rightVec             ;
    m_frustum.right.normal  = rightVec.getNegated();
    */

    // Then move the camera.
    Mat4 viewMat = getViewMatrix();
    Vector3 dir;

    // Set direction accoring to inputs.
    if (inputs.moveForward)  dir.z = -1.f;
    if (inputs.moveBackward) dir.z =  1.f;
    if (inputs.moveLeft)     dir.x = -1.f;
    if (inputs.moveRight)    dir.x =  1.f;
    if (inputs.moveUpper)    dir.y = -1.f;
    if (inputs.moveLower)    dir.y =  1.f;

    // Set speed according to deltatime.
    m_speed = deltaTime * m_acceleration;

    m_pos += getSphericalCoords(m_speed, PI/2, 2*PI - m_pitch)        * dir.x
          +  getSphericalCoords(m_speed, PI/2, 2*PI - m_pitch - PI/2) * dir.z;
    m_pos.y += dir.y * m_speed;
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
    ImGui::SliderFloat("Speed", &m_acceleration, 1.f, 20.f);
}