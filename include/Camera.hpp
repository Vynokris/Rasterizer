#pragma once

#include <my_math.hpp>

struct CameraInputs
{
    float deltaX, deltaY;
    bool moveForward, moveBackward, moveLeft,
         moveRight, moveUpper, moveLower;
};

class Camera
{
private:
    unsigned int width, height;

    float fov, aspect, near, far,
          pitch, yaw, speed, acceleration;
          
    Vector3 pos, direction;

public:
    Camera(const unsigned int _width, const unsigned int _height,
           const float _fov, const float _near, const float _far,
           const float _acceleration);

    // Main methods.
    void update(const float _deltaTime, const CameraInputs& _inputs);

    // Getters.
    Mat4                getWorldTransform() const;
    float               getPitch()          const;
    float               getYaw()            const;
    float               getNear()           const;
    float               getFar()            const;
    matrix::Mat4        getProjection()     const;
    matrix::Mat4        getViewMatrix()     const;

    // Setters.
    void setPosition(const Vector3& _pos);
    void setRotation(const float _pitch, const float _yaw);

    // Misc.
    void showImGuiControls();
};