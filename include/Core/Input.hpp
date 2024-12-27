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
    COUNT
};

struct InputEvent : public Event {
    std::bitset<static_cast<size_t>(InputType::COUNT)> inputs;

    explicit InputEvent(std::bitset<static_cast<size_t>(InputType::COUNT)> _inputs)
        : inputs(_inputs) {}
};

// For debugging purposes, returns the key pressed in string form
static std::string GetInputTypeName(InputType inputType) {
    switch (inputType) {
        case InputType::MouseLeft:
            return "Left Mouse";
        case InputType::MouseRight:
            return "Right Mouse";
        case InputType::KeyW:
            return "W Key";
        case InputType::KeyA:
            return "A Key";
        case InputType::KeyS:
            return "S Key";
        case InputType::KeyD:
            return "D Key";
        default:
            return "Unknown Input";
    }
}

#endif //INPUT_HPP
