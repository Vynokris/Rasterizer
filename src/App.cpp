#include <string>
#include "App.hpp"
#include "Renderer.hpp"
#include "Framebuffer.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

using namespace matrix;

//Get back Events and setup ImGUI frame
void App::newFrame(bool _mouseCaptured)
{
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    if (_mouseCaptured)
        ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    ImGui::NewFrame();
}

//Clear buffer et render ImGUI
void App::endFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

//Constructor with Init struct
App::App(const AppInit& _init)
{
    // Setup glfw
    glfwSetErrorCallback(_init.ErrorCallback);
    if (!glfwInit())
    {
        printf("glfwInit failed.\n");
        state = GLFWFAILED;
        return;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    window = glfwCreateWindow(_init.width, _init.height, _init.title, nullptr, nullptr);
    if (window == nullptr)
    {
        printf("glfwCreateWindow failed.\n");
        state = WINDOWFAILED;
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync
    
    // Setup glad
    if (gladLoadGL((GLADloadfunc)glfwGetProcAddress) == 0)
    {
        printf("gladLoaderLoadGL failed.\n");
        state = GLADFAILED;
        return;
    }

    // Setup KHR debug
    if (GLAD_GL_KHR_debug)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(_init.DebugMessageCallback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
    }

    printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GL Renderer: %s\n", glGetString(GL_RENDERER));

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::LoadIniSettingsFromDisk("externals/include/imgui.ini");
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    state = SUCCESS;
}

//Update device and call the renderer
void App::update()
{
    // Initialize app objects.
    Scene scene;

    Material emerald = { WHITE, WHITE, 0.2, 1 };

    Renderer renderer(800, 800, scene.getLights());
    renderer.setMaterial(emerald);

    CameraInputs inputs;
    Camera camera(renderer.framebuffer.getWidth(), renderer.framebuffer.getHeight(), 90.f, 0.001f, 1000.f, 2.5f);

    // Load a texture.
    TextureData baseTexture = loadBmpData("art/UrAppreciated.bmp");
    // renderer.setTexture(baseTexture);

    bool mouseCaptured = false;
    double mouseX = 0.0, mouseY = 0.0;
    float mouseDeltaX = 0.0, mouseDeltaY = 0.0;

    while (glfwWindowShouldClose(window) == false)
    {
        newFrame(mouseCaptured);

        // Update the mouse pos and delta.
        {
            double newMouseX, newMouseY;
            glfwGetCursorPos(window, &newMouseX, &newMouseY);
            mouseDeltaX = (float)(newMouseX - mouseX);
            mouseDeltaY = (float)(newMouseY - mouseY);
            mouseX = newMouseX;
            mouseY = newMouseY;
        }

        // Update the exit key.
        if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) break;

        // Update the camera position and rotation.
        if (mouseCaptured)
        {
            inputs.deltaX = mouseDeltaX;
            inputs.deltaY = mouseDeltaY;
            inputs.moveForward  = ImGui::IsKeyDown(GLFW_KEY_W);
            inputs.moveBackward = ImGui::IsKeyDown(GLFW_KEY_S);
            inputs.moveLeft     = ImGui::IsKeyDown(GLFW_KEY_A);
            inputs.moveRight    = ImGui::IsKeyDown(GLFW_KEY_D);
            inputs.moveUpper    = ImGui::IsKeyDown(GLFW_KEY_SPACE);
            inputs.moveLower    = ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT);
            camera.update(ImGui::GetIO().DeltaTime, inputs);
        }

        // Clear buffers
        renderer.framebuffer.clear(camera.getFar());

        // Setup matrices
        renderer.setProjection(camera.getPerspective());
        renderer.setView(camera.getViewMat());

        // Render scene
        scene.update(ImGui::GetIO().DeltaTime, renderer, camera);

        // Update texture
        renderer.framebuffer.updateTexture();

        // Display info.
        if (ImGui::Begin("Info"))
        {
            // Compute smooth FPS.
            static int counter = 0;
            static float deltaTimes = 0, fps = 0;
            counter++;
            deltaTimes += ImGui::GetIO().DeltaTime;
            if (counter >= 10)
            {
                counter = 0;
                fps = 1 / (deltaTimes / 10);
                deltaTimes = 0;
            }

            // Show FPS and frame duration.
            ImGui::Text("FPS: %.f", fps);
            ImGui::Text("Frame duration: %f seconds.", ImGui::GetIO().DeltaTime);
        }
        ImGui::End();

        // Display debug controls.
        if (ImGui::Begin("Config"))
        {
            if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
            {
                renderer.showImGuiControls();
            }
            if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
            {
                scene.showImGuiControls();
            }
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                camera.showImGuiControls();
            }
        }
        ImGui::End();
        
        // Display the rasterizer's output.
        ImGui::Begin("Render");
        {
            if (mouseCaptured == false) ImGui::Text("(Hold RMB to capture mouse)");
            else                        ImGui::Text("(Release RMB to leave mouse capture mode)");

            ImGui::Image((ImTextureID)(size_t)renderer.framebuffer.getColorTexture(), { (float)renderer.framebuffer.getWidth(), (float)renderer.framebuffer.getHeight() });
            if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                mouseCaptured = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            {
                mouseCaptured = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        ImGui::End();

        endFrame();
    }

    //! Temporary.
    delete[] baseTexture.pixels;
}

//destructor who kill glfw and clean the window buffer
App::~App()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
