
#include <imgui.h>
#include <my_math.hpp>
#include <Camera.hpp>

using namespace arithmetic;
using namespace matrix;
using namespace geometry3D;

Camera::Camera(const unsigned int _width, const unsigned int _height,
               const float _fov, const float _near, const float _far,
               const float _acceleration)
      : width(_width), height(_height),
        fov(_fov), near(_near), far(_far),
        acceleration(_acceleration)
{
    aspect = (float)width / height;
    pitch = yaw = speed = 0.f;
}

//* ---------- MAIN CAMERA METHODS ---------- *//

void Camera::update(const float _deltaTime, const CameraInputs& _inputs)
{
    // Rotate camera (Yaw locked between -90° and 90°, Pitch reset to 0 when it reaches 360°).
    setRotation(fmodf(pitch + _inputs.deltaX / 180.f, 2*PI), 
                clamp(yaw   - _inputs.deltaY / 180.f, -PI/2 + 0.001, PI/2 - 0.001));

    // Set direction accoring to inputs.
    Vector3 dir;
    if (_inputs.moveForward)  dir.z += -1.f;
    if (_inputs.moveBackward) dir.z +=  1.f;
    if (_inputs.moveLeft)     dir.x += -1.f;
    if (_inputs.moveRight)    dir.x +=  1.f;
    if (_inputs.moveUpper)    dir.y += -1.f;
    if (_inputs.moveLower)    dir.y +=  1.f;

    // Set speed according to deltatime.
    speed = _deltaTime * acceleration;

    pos   += getSphericalCoords(speed, PI/2, 2*PI - pitch)        * dir.x
          +  getSphericalCoords(speed, PI/2, 2*PI - pitch - PI/2) * dir.z;
    pos.y += dir.y * speed;
}

//* -------- CAMERA GETTER METHODS --------- *//

Mat4     Camera::getWorldTransform() const { return getViewMat().inv4(); }
float    Camera::getPitch()          const { return pitch;    }
float    Camera::getYaw()            const { return yaw;      }
float    Camera::getNear()           const { return near;     }
float    Camera::getFar()            const { return far;      }
ViewMode Camera::getViewMode()       const { return viewMode; }

Mat4 Camera::getPerspective() const
{
    float yScale = 1.f / tan(fov / 2.f);
    float xScale = yScale / aspect;

    return Mat4(
        xScale, 0,      0,                         0,
        0,      yScale, 0,                         0,
        0,      0,      far        / (far - near), 1,
        0,      0,    - far * near / (far - near), 0
    );
}

Mat4 Camera::getOrthographic() const
{
    // TODO: fix this.
    float r =  (float)width  / 480;
    float l = -(float)width  / 480;
    float t =  (float)height / 480;
    float b = -(float)height / 480;
    float n = near;
    float f = far;

    return Mat4(
        2/(r - l), 0,         0,            -(r + l) / (r - l),
        0,         2/(t - b), 0,            -(t + b) / (t - b),
        0,         0,         -2 / (f - n), -(f + n) / (f - n),
        0,         0,         0,            1
    );
}

Mat4 Camera::getViewMat() const
{
    Mat4 output(true);
    return output * getTransformMatrix(pos.getNegated(), { yaw, pitch, 0 }, { 1, 1, 1 }, true);
}

Mat4 Camera::getLookAtMat(const Vector3& _target) const
{
    Vector3 zAxis = (_target - pos).getNormalized();
    Vector3 xAxis = (Vector3{0, 1, 0} ^ zAxis).getNormalized();
    Vector3 yAxis = (zAxis ^ xAxis).getNormalized();

    return Mat4(
        xAxis.x, yAxis.x, zAxis.x, 0,
        xAxis.y, yAxis.y, zAxis.y, 0,
        xAxis.z, yAxis.z, zAxis.z, 0,
        xAxis & pos.getNegated(), yAxis & pos.getNegated(), zAxis & pos.getNegated(), 1
    );
}

//* -------- CAMERA SETTERS METHODS --------- *//

void Camera::setPosition(const Vector3& _pos)
{
    pos = _pos;
}

void Camera::setRotation(const float _pitch, const float _yaw)
{
    pitch = _pitch; yaw = _yaw;
}

void Camera::setViewMode(const ViewMode& _viewMode)
{
    viewMode = _viewMode;
}

void Camera::setLookAtRotation(const Vector3& _target)
{
    Vector3 zAxis = (_target - pos).getNormalized();
    setRotation(zAxis.getAnglePhi(), zAxis.getAngleTheta());
}

//* ------------------ MISC. ---------------- *//

void Camera::showImGuiControls()
{
    // Static variables.
    static const char* items[]{"1st person", "3rd person"};
    static int curItem = 0;

    ImGui::Combo("View Mode", &curItem, items, IM_ARRAYSIZE(items));
    setViewMode((ViewMode)curItem);
    ImGui::Text("Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
    ImGui::Text("Pitch: %.2f° | Yaw = %.2f°", radToDeg(pitch), radToDeg(yaw));
    ImGui::SliderFloat("Speed", &acceleration, 1.f, 20.f);
}