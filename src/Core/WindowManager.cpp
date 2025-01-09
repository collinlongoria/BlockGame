//
// Created by colli on 12/26/2024.
//

#include "Core/WindowManager.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ECS/Coordinator.hpp"

extern Coordinator coordinator;

void WindowManager::QuitHandler(const QuitEvent &Event) {
    toQuit = true;
    Log::Output(Log::Severity::HAPPY, "Quitting...");
}


void WindowManager::Init(std::string const &windowName, unsigned int windowWidth, unsigned int windowHeight, unsigned int windowPosX, unsigned int windowPosY) {
    // Init GLFW
    if(!glfwInit()) {
        Log::Output(Log::Severity::ERROR, "Failed to initialize GLFW");
        toQuit = true;
        return;
    }

    // Create window
    window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
    if(!window) {
        Log::Output(Log::Severity::ERROR, "Failed to create GLFW window.");
        toQuit = true;
        return;
    }

    // specify version
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make context current
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Log::Output(Log::Severity::ERROR, "Failed to initialize GLAD.");
        toQuit = true;
        return;
    }
    glfwSwapInterval(2);

    // Configure OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);

    // Disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouseVisible = false;

    // Enable quit event
    coordinator.AddEventListener<QuitEvent>(this, &WindowManager::QuitHandler);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Log::Output(Log::Severity::HAPPY, "Window initialised!");
}

void WindowManager::Update() {
    // render imgui panels
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void WindowManager::ProcessInput() {
    glfwPollEvents();

    if(glfwWindowShouldClose(window)) {
        toQuit = true;
    }

    // Reset input states and capture mouse position changes
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float deltaX = 0.0f, deltaY = 0.0f;

    if (mouseMoved) {
        deltaX = static_cast<float>(mouseX) - mouseDeltaX;
        deltaY = static_cast<float>(mouseY) - mouseDeltaY;
    } else {
        mouseMoved = true;
    }

    mouseDeltaX = static_cast<float>(mouseX);
    mouseDeltaY = static_cast<float>(mouseY);

    UpdateInput();

    // Dispatch if there was any input
    // TODO: Only send input event if input has changed
    coordinator.DispatchEvent(InputEvent(input, deltaX, deltaY, mouseVisible));
}

void WindowManager::Shutdown() {

    // imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::UpdateInput() {

    // putting this here because this function is called prior to other systems
    // TODO: move to it's own function
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // save previous frames input
    static std::bitset<static_cast<size_t>(InputType::COUNT)> previousInput;

    // reset bits
    // maybe bool is faster?
    input.reset();

    // Update input states
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyEscape))) {
            // Single click events
            coordinator.DispatchEvent(QuitEvent());
        }
        input.set(static_cast<size_t>(InputType::KeyEscape));
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
       if (!previousInput.test(static_cast<size_t>(InputType::MouseLeft))) {
           // Single click events
       }
        input.set(static_cast<size_t>(InputType::MouseLeft));
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::MouseRight))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::MouseRight));
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
       if (!previousInput.test(static_cast<size_t>(InputType::KeyW))) {
           // Single click events
       }
        input.set(static_cast<size_t>(InputType::KeyW));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyS))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeyS));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyA))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeyA));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyD))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeyD));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeySpace))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeySpace));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyLeftCtrl))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeyLeftCtrl));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyLeftAlt))) {
            // Single click events
            mouseVisible = !mouseVisible;
            Log::Output(Log::Severity::WARNING, "ALT CLICKED! Mouse set to " + std::to_string(mouseVisible));
            glfwSetInputMode(window, GLFW_CURSOR, mouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        input.set(static_cast<size_t>(InputType::KeyLeftAlt));
    }
    if(glfwGetKey(window, GLFW_KEY_TAB == GLFW_PRESS)) {
        if (!previousInput.test(static_cast<size_t>(InputType::KeyTab))) {
            // Single click events
        }
        input.set(static_cast<size_t>(InputType::KeyTab));
    }

    previousInput = input;
}




