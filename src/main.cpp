#include <cstdio>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "App.hpp"


int main(int argc, char* argv[])
{
    // Prepare the initialization structure.
    AppInit init =
    {
        1920, 1080, 
        "Alemi Engine",
        [](int errorCode, const char* description)
        { 
            printf("GLFW error (code=%d): '%s'\n", errorCode, description);
        },
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
        {
            printf("OpenGL error: '%s'\n", message);
        }
    };

    // Initialize the app.
    App myApp(init);

    // Check app state.
    switch (myApp.getState())
    {
    case EAppState::NONE:
    case EAppState::GLFWFAILED:
    case EAppState::WINDOWFAILED:
    case EAppState::GLADFAILED:
        throw;
    case EAppState::SUCCESS:
        break;
    default:
        break;
    }

    myApp.update();


    return 0;
}