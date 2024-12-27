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

    std::bitset<static_cast<size_t>(InputType::COUNT)> input;
};



#endif //WINDOWMANAGER_HPP
