//
// Created by colli on 12/27/2024.
//

#ifndef INPUT_HPP
#define INPUT_HPP

#include <bitset>

#include "ECS/Event.hpp"

// enum for input types
enum class InputType {
    MouseLeft,
    MouseRight,
    KeyW,
    KeyA,
    KeyS,
    KeyD,
    KeySpace,
    KeyLeftCtrl,
    KeyEscape,
    COUNT
};

struct InputEvent : public Event {
    std::bitset<static_cast<size_t>(InputType::COUNT)> inputs;
    float mouseDeltaX;
    float mouseDeltaY;

    explicit InputEvent(std::bitset<static_cast<size_t>(InputType::COUNT)> _inputs, float dX, float dY)
        : inputs(_inputs), mouseDeltaX(dX), mouseDeltaY(dY) {}
};

#endif //INPUT_HPP
