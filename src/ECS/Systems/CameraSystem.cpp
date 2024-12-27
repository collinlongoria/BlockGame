//
// Created by colli on 12/27/2024.
//

#include "ECS/Systems/CameraSystem.hpp"
#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Transform.hpp"

#include "ECS/Coordinator.hpp"

extern Coordinator coordinator;

void CameraSystem::Init() {
    coordinator.AddEventListener<InputEvent>(this, &CameraSystem::InputListener);
}

void CameraSystem::Update(float dt) {
    for(auto& entity : entities) {
        auto& transform = coordinator.GetComponent<Transform>(entity);

        float speed = 10.0f;

        // Z axis (forward/backward)
        if(input.test(static_cast<size_t>(InputType::KeyW))) {
            transform.position.z -= (speed * dt);
        }
        else if(input.test(static_cast<size_t>(InputType::KeyS))) {
            transform.position.z += (speed * dt);
        }

        // X axis (left/right)
        if(input.test(static_cast<size_t>(InputType::KeyA))) {
            transform.position.x -= (speed * dt);
        }
        else if(input.test(static_cast<size_t>(InputType::KeyD))) {
            transform.position.x += (speed * dt);
        }

        // Y axis (up/down)
        if(input.test(static_cast<size_t>(InputType::MouseLeft))) {
            transform.position.y -= (speed * dt);
        }
        else if(input.test(static_cast<size_t>(InputType::MouseRight))) {
            transform.position.y += (speed * dt);
        }
    }
}

void CameraSystem::InputListener(const InputEvent &event) {
    input = event.inputs;
}


