//
// Created by colli on 12/26/2024.
//

#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>

#include "Input.hpp"

struct QuitEvent : public Event {

};

class WindowManager {
public:
    void Init(std::string const& windowName,
        unsigned int windowWidth, unsigned int windowHeight,
        unsigned int windowPosX, unsigned int windowPosY);

    void Update();

    void ProcessInput();

    void Shutdown();

    // Poll window manager for quit bool
    bool Quit() const {return toQuit;}

private:
    GLFWwindow* window;

    bool toQuit = false;
    bool mouseVisible = false;

    // Wrapper helper function for updating key log bitfield
    void UpdateInput();

    // Quit event function
    void QuitHandler(const QuitEvent& Event);

    // Bitfield for key inputs (defined in Input.hpp)
    std::bitset<static_cast<size_t>(InputType::COUNT)> input;

    // Data related to mouse movements
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;
    bool mouseMoved = false;
};



#endif //WINDOWMANAGER_HPP
