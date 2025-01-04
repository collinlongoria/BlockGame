//
// Created by colli on 12/27/2024.
//

#include "ECS/Systems/CameraSystem.hpp"

#include <iostream>

#include "ECS/Components/Camera.hpp"
#include "ECS/Components/Transform.hpp"

#include "ECS/Coordinator.hpp"

extern Coordinator coordinator;

float sensitivity = 0.2f;

void CameraSystem::Init() {
    coordinator.AddEventListener<InputEvent>(this, &CameraSystem::InputListener);
}

glm::vec3 CalculateForward(float yaw, float pitch) {
    glm::vec3 forward;
    forward.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); // Swap sin/cos for OpenGL
    forward.y = sin(glm::radians(pitch));
    forward.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch)); // Negative Z for forward
    return glm::normalize(forward);
}

glm::vec3 CalculateRight(const glm::vec3& forward) {
    // Cross the forward vector with the world up (0, 1, 0) to get the right vector
    return glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 CalculateUp(const glm::vec3& forward, const glm::vec3& right) {
    // Cross the right and forward vectors to get the up vector
    return glm::normalize(glm::cross(right, forward));
}

void CameraSystem::Update(float dt) {
    for(auto& entity : entities) {
        auto& transform = coordinator.GetComponent<Transform>(entity);

        float speed = 10.0f;

        glm::vec3 forward = CalculateForward(transform.rotation.y, transform.rotation.x); // Yaw and Pitch
        glm::vec3 right = CalculateRight(forward);
        glm::vec3 up = CalculateUp(forward, right);

        // Z axis (forward/backward)
        if (input.test(static_cast<size_t>(InputType::KeyW))) {
            transform.position += forward * speed * dt; // Move forward
        }
        if (input.test(static_cast<size_t>(InputType::KeyS))) {
            transform.position -= forward * speed * dt; // Move backward
        }
        if (input.test(static_cast<size_t>(InputType::KeyA))) {
            transform.position -= right * speed * dt; // Move left
        }
        if (input.test(static_cast<size_t>(InputType::KeyD))) {
            transform.position += right * speed * dt; // Move right
        }

        // Y axis (up/down)
        if(input.test(static_cast<size_t>(InputType::KeySpace))) {
            transform.position.y -= (speed * dt);
        }
        else if(input.test(static_cast<size_t>(InputType::KeyLeftCtrl))) {
            transform.position.y += (speed * dt);
        }

        // Rotation
        if (mouseDeltaX != 0.0f || mouseDeltaY != 0.0f) {
            // Rotation
            transform.rotation.x -= mouseDeltaY * sensitivity; // Pitch
            transform.rotation.y += mouseDeltaX * sensitivity; // Yaw

            // Clamp pitch to avoid flipping
            transform.rotation.x = glm::clamp(transform.rotation.x, -89.0f, 89.0f);
        }
    }
}

void CameraSystem::InputListener(const InputEvent &event) {
    input = event.inputs;
    mouseDeltaX = event.mouseDeltaX;
    mouseDeltaY = event.mouseDeltaY;
}


