#pragma once
#include <cstdio>
#include <imgui.h>

#include <glad/gl.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Framebuffer.hpp>

//Structure d'initialisation de l'app
struct AppInit
{
    int width, height; 
    const char* title;
    void (*ErrorCallback)(int errorCode, const char* description);
    void (*DebugMessageCallback)(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                 GLsizei length, const GLchar* message, const void* userParam);
};

//Enum state of Init app
enum EAppState { NONE, GLFWFAILED, WINDOWFAILED, GLADFAILED, SUCCESS };

//Unique Class of l'app
class App
{
private:
    // Tracks the state of the app.
    EAppState state = NONE;

public:
    // The app's main window.
    GLFWwindow* window;

    // Constructor/Destructor.
    App(const AppInit& p_init);
    ~App();

    // The app's main loop.
    void update();

    // Getter.
    inline EAppState getState() { return state; }

private:
    // Frame creation and addition.
    void newFrame(bool mouseCaptured = true);
    void endFrame();
};
