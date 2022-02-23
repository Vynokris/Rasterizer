
#include <imgui.h>
#include <my_math.hpp>
#include <Camera.hpp>
#include <cstdio>

using namespace arithmetic;
using namespace matrix;
using namespace geometry3D;

Camera::Camera(const unsigned int& _width,
               const unsigned int& _height,
               const float& _fov, const float& _near,
               const float& _far, const float& _acceleration)
    : width       (_width) ,
      height      (_height),
      fov         (_fov)   ,
      near        (_near)  ,
      far         (_far)   ,
      acceleration(_acceleration)
{
    aspect = (float) width / height;
    pitch = yaw = speed = 0;
    lookAtPoint.z = 2;
}

void Camera::update(const float& _deltaTime, const CameraInputs& _inputs)
{
    // Set speed according to deltatime.
    speed = _deltaTime * acceleration;

    switch(viewMode)
    {
    case ViewMode::FIRST_PERSON:
        // Rotate camera (Yaw locked between -90° and 90°, Pitch reset to 0 when it reaches 360°).
        setRotation(fmodf(pitch + _inputs.deltaX / 180, 2*PI), 
                    clamp(yaw   - _inputs.deltaY / 180, -PI/2 + 0.001, PI/2 - 0.001));
        break;
    case ViewMode::THIRD_PERSON:
        // Update the camera's rotation to be towards the target.
        setLookAtRotation();
        break;
    default:
        break;
    }

    // Set direction accoring to inputs.
    Vector3 dir;
    if (_inputs.moveBackward) dir.z +=  1;
    if (_inputs.moveLeft)     dir.x += -1;
    if (_inputs.moveRight)    dir.x +=  1;
    if (_inputs.moveUpper)    dir.y += -1;
    if (_inputs.moveLower)    dir.y +=  1;
    if (_inputs.moveForward)  dir.z += -1;

    // Update position.
    pos   += getSphericalCoords(speed, PI/2, 2*PI - pitch)        * dir.x
            +  getSphericalCoords(speed, PI/2, 2*PI - pitch - PI/2) * dir.z;
    pos.y += dir.y * speed;
}

Mat4     Camera::getWorldTransform() const { return getViewMat().inv4(); }
float    Camera::getPitch()          const { return pitch;    }
float    Camera::getYaw()            const { return yaw;      }
float    Camera::getNear()           const { return near;     }
float    Camera::getFar()            const { return far;      }
ViewMode Camera::getViewMode()       const { return viewMode; }

Mat4 Camera::getPerspective() const
{
    float yScale = 1 / tan(fov / 2);
    float xScale = yScale / aspect;

    return Mat4(
        xScale, 0,      0,                         0,
        0,      yScale, 0,                         0,
        0,      0,      far        / (far - near), 1,
        0,      0,    - far * near / (far - near), 0
    );
}

Mat4 Camera::getViewMat() const
{
    Mat4 output(true);
    return output * getTransformMatrix(-pos, { yaw, pitch, 0 }, { 1, 1, 1 }, true);
}

Mat4 Camera::getLookAtMat() const
{
    Vector3 zAxis = (lookAtPoint - pos).getNormalized();
    Vector3 xAxis = (Vector3{0, 1, 0} ^ zAxis).getNormalized();
    Vector3 yAxis = (zAxis ^ xAxis).getNormalized();

    return Mat4(
        xAxis.x,      yAxis.x,      zAxis.x,      0,
        xAxis.y,      yAxis.y,      zAxis.y,      0,
        xAxis.z,      yAxis.z,      zAxis.z,      0,
        xAxis & -pos, yAxis & -pos, zAxis & -pos, 1
    );
}

void Camera::setPosition(const Vector3& _pos)                    { pos      = _pos;               }
void Camera::setRotation(const float& _pitch, const float& _yaw) { pitch    = _pitch; yaw = _yaw; }
void Camera::setViewMode(const ViewMode& _viewMode)              { viewMode = _viewMode;          }
void Camera::setLookAtPoint(const Vector3& _target)              { lookAtPoint = _target;         }

void Camera::setLookAtRotation()
{
    Vector3 zAxis = (lookAtPoint - pos).getNormalized();
    setRotation(fmod(zAxis.getAngleTheta(), 2*PI), 
                fmod(zAxis.getAnglePhi(),   2*PI));
}

void Camera::showImGuiControls()
{
    // View mode combo box.
    static const char* items[]{"1st person", "3rd person"};
    static int curItem = 0;

    ImGui::Combo("View Mode", &curItem, items, IM_ARRAYSIZE(items));
    setViewMode((ViewMode)curItem);

    // Displayed camera members.
    ImGui::Text("Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
    ImGui::Text("Pitch: %.2f° | Yaw = %.2f°", radToDeg(pitch), radToDeg(yaw));
    ImGui::SliderFloat("Speed", &acceleration, 0.05, 10);
    if (viewMode == ViewMode::THIRD_PERSON)
        ImGui::SliderFloat3("Look at point", &lookAtPoint.x, -5, 5);
}