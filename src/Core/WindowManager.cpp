//
// Created by colli on 12/26/2024.
//

#include "Core/WindowManager.hpp"

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
    glDisable(GL_CULL_FACE);

    // Disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable quit event
    coordinator.AddEventListener<QuitEvent>(this, &WindowManager::QuitHandler);

    Log::Output(Log::Severity::HAPPY, "Window initialised!");
}

void WindowManager::Update() {
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
    coordinator.DispatchEvent(InputEvent(input, deltaX, deltaY));
}

void WindowManager::Shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowManager::UpdateInput() {
    // reset bits
    // maybe bool is faster?
    input.reset();

    // Update input states
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        coordinator.DispatchEvent(QuitEvent());
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        input.set(static_cast<size_t>(InputType::MouseLeft));
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
        input.set(static_cast<size_t>(InputType::MouseRight));
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeyW));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeyS));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeyA));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeyD));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeySpace));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        input.set(static_cast<size_t>(InputType::KeyLeftCtrl));
    }
}




