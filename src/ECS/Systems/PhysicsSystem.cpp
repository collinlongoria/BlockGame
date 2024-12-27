//
// Created by colli on 12/26/2024.
//

#include "ECS/Systems/PhysicsSystem.hpp"

#include "ECS/Components/Gravity.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Coordinator.hpp"

extern Coordinator coordinator;

void PhysicsSystem::Init() {

}

void PhysicsSystem::Update(float dt) {
    for(auto const& entity : entities) {
        auto& rigidBody = coordinator.GetComponent<RigidBody>(entity);
        auto& transform = coordinator.GetComponent<Transform>(entity);
        auto& gravity = coordinator.GetComponent<Gravity>(entity);

        transform.position += rigidBody.velocity * dt;

        rigidBody.velocity += gravity.force * dt;
    }
}


