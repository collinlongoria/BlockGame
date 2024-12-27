//
// Created by colli on 12/26/2024.
//

#ifndef COMPONENTARRAY_HPP
#define COMPONENTARRAY_HPP
#include "Entity.hpp"
#include "Core/Log.hpp"

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    void InsertData(Entity entity, T component) {
        if(entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            Log::Output(Log::Severity::ERROR, "Entity already has component.");
            return;
        }

        // Insert entry at end and update map
        size_t index = size;
        entityToIndexMap[entity] = index;
        indexToEntityMap[index] = entity;
        componentArray[index] = component;
        size++;
    }

    void RemoveData(Entity entity) {
        if(entityToIndexMap.find(entity) == entityToIndexMap.end()) {
            Log::Output(Log::Severity::ERROR, "Removing non-existent component.");
            return;
        }

        // Copy element at end into deleted element's place
        // Goal is to maintain density
        size_t indexRemoved = entityToIndexMap[entity];
        size_t indexLast = size - 1;
        componentArray[indexRemoved] = componentArray[indexLast];

        // Update the map
        Entity entityLast = indexToEntityMap[indexLast];
        entityToIndexMap[entityLast] = indexRemoved;
        indexToEntityMap[indexRemoved] = entityLast;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexLast);

        size--;
    }

    T& GetData(Entity entity) {
        if(entityToIndexMap.find(entity) == entityToIndexMap.end()) {
            Log::Output(Log::Severity::ERROR, "Entity does not have component.");
            static T defaultComponent = {};
            return defaultComponent;
        }

        // Return reference to the component
        return componentArray[entityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) {
        if(entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            // Remove entity's component if it exists
            RemoveData(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES> componentArray;
    std::unordered_map<Entity, size_t> entityToIndexMap;
    std::unordered_map<size_t, Entity> indexToEntityMap;
    size_t size;
};



#endif //COMPONENTARRAY_HPP
