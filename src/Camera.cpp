
#include <imgui.h>

#include <Camera.hpp>
using namespace matrix;

Camera::Camera(const unsigned int width, const unsigned int height)
{
    //TODO
    // width and height are useful to compute projection matrix with the right aspect ratio
}

void Camera::Update(const float p_deltaTime, const CameraInputs& inputs)
{
    //TODO
}

Mat4 Camera::GetViewMatrix()
{
    //TODO
    return Mat4(true);
}

Mat4 Camera::GetProjection()
{
    //TODO
    return Mat4(true);
}

void Camera::ShowImGuiControls()
{
    //TODO
}