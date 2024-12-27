//
// Created by colli on 12/27/2024.
//

#ifndef CAMERASYSTEM_HPP
#define CAMERASYSTEM_HPP

#include <ECS/System.hpp>

#include "Core/Input.hpp"

class CameraSystem : public System {
public:
    void Init();

    void Update(float dt);

private:
    std::bitset<static_cast<size_t>(InputType::COUNT)> input;

    void InputListener(const InputEvent& event);
};



#endif //CAMERASYSTEM_HPP
