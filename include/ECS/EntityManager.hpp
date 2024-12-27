//
// Created by colli on 12/26/2024.
//

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <queue>
#include "Component.hpp"
#include "Entity.hpp"
#include "Core/Math.hpp"
#include "Core/Log.hpp"

class EntityManager {
public:
    EntityManager() {
        // Init the queue with all possible entity IDs
        for(Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
            availableEntities.push(entity);
        }
    }

    ~EntityManager() {
        // Nothing for now
    }

    Entity CreateEntity() {
        if(livingEntities >= MAX_ENTITIES) {
            Log::Output(Log::Severity::ERROR, "Too many entities!");
            return -1; // error?
        }

        // Get ID from front of the queue
        Entity id = availableEntities.front();
        availableEntities.pop();
        ++livingEntities;

        return id;
    }

    void DestroyEntity(Entity entity) {
        if(entity >= MAX_ENTITIES) {
            Log::Output(Log::Severity::ERROR, "ID out of range.");
            return;
        }

        // Invalidate signature
        signatures[entity].reset();

        // Put destroyed ID at the back of the queue
        availableEntities.push(entity);
        --livingEntities;
    }

    void SetSignature(Entity entity, Signature signature) {
        if(entity >= MAX_ENTITIES) {
            Log::Output(Log::Severity::ERROR, "ID out of range.");
            return;
        }

        // put this entity's signature into the array
        signatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) {
        if(entity >= MAX_ENTITIES) {
            Log::Output(Log::Severity::ERROR, "ID out of range.");
            return -1; // error?
        }

        // Get entity's signature from the array
        return signatures[entity];
    }

private:
    // queue of unused entity IDs
    std::queue<Entity> availableEntities{};

    // array of signatures (index corresponds to entity ID)
    std::array<Signature, MAX_ENTITIES> signatures{};

    // total living entities
    uint32_t livingEntities{};
};

#endif //ENTITYMANAGER_HPP
