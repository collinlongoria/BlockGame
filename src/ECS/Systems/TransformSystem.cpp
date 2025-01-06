//
// Created by colli on 1/6/2025.
//

#include "ECS/Systems/TransformSystem.hpp"

#include "ECS/Coordinator.hpp"
#include "ECS/Components/Transform.hpp"

extern Coordinator coordinator;

void TransformSystem::Init() {
    // nothing currently here
}

void TransformSystem::Update(float dt) {
    // For each entity, update the transform if needed
    for(auto& entity : entities) {
        auto& transform = coordinator.GetComponent<Transform>(entity);

        // this won't work if glm does not properly compare floats. I am assuming it does
        if(
            transform.position != transform.old_position ||
            transform.rotation != transform.old_rotation ||
            transform.scale != transform.old_scale) {

            // update the matrix
            transform.matrix = glm::translate(glm::mat4(1.0f), transform.position) *
                              glm::scale(glm::mat4(1.0f), transform.scale) *
                              glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);

            // update the old positions
            transform.old_position = transform.position;
            transform.old_scale = transform.scale;
            transform.old_rotation = transform.rotation;
        }
    }
}

