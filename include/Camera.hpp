#pragma once

#include <my_math.hpp>

enum class ViewMode       : int { FIRST_PERSON, THIRD_PERSON };
enum class LookAtMovement : int { MOUSE, KEYBOARD };

struct CameraInputs
{
    float deltaX, deltaY, mouseWheel;
    bool moveForward, moveBackward, 
         moveLeft,    moveRight, 
         moveUp,      moveDown;
};

class Camera
{
private:
    unsigned int width, height;

    float fov, aspect, near, far,
          pitch, yaw, speed, acceleration,
          lookAtDist = 2;
          
    Vector3 pos, direction, lookAtPoint;

    ViewMode       viewMode       = ViewMode::FIRST_PERSON;
    LookAtMovement lookAtMovement = LookAtMovement::MOUSE;

public:
    Camera(const unsigned int& _width, const unsigned int& _height,
           const float& _fov, const float& _near, const float& _far,
           const float& _accelerations);

    // Main methods.
    void update(const float& _deltaTime, CameraInputs _inputs);

    // Getters.
    Mat4     getWorldTransform() const;
    float    getPitch()          const;
    float    getYaw()            const;
    float    getNear()           const;
    float    getFar()            const;
    ViewMode getViewMode()       const;

    // Returns the perspective projection matrix.
    matrix::Mat4 getPerspective()  const;

    // Returns the view transformation matrix.
    matrix::Mat4 getViewMat()      const;

    // Returns the look at transformation matrix.
    matrix::Mat4 getLookAtMat() const;

    // Setters.
    void setPosition      (const Vector3& _pos);
    void setRotation      (const float& _pitch, const float& _yaw);
    void setViewMode      (const ViewMode& _viewMode);
    void setLookAtPoint   (const Vector3& _target);

    // Sets the rotation of the camera to match the lookAt matrix rotation.
    void setLookAtRotation();

    // Misc.
    void showImGuiControls();
};